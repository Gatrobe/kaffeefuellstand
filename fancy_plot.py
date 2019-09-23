#!/usr/bin/python3

import sqlite3
import matplotlib

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.dates import DateFormatter
import numpy as np
import requests
from datetime import datetime, timedelta
from email.utils import mktime_tz, parsedate_tz
from dateutil import tz

import os

API_URL = 'https://shop.gatrobe.de/api'
COFFEE_PRODUCT_ID = 53

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

conn = sqlite3.connect('coffee.db', detect_types=sqlite3.PARSE_DECLTYPES)
c = conn.cursor()

# status: "unreachable", "no coffeepot", "ok"
STATUS = {
    "ok": 0,
    "no coffeepot": 1,
    "unreachable": 2
}

consumers = None


def get_consumer(consumer_id):
    global consumers
    if consumers is None:
        consumers = requests.get(API_URL + '/users').json()['users']

    return next(
        map(lambda x: x['firstname'] + ' ' + x['lastname'], filter(lambda x: x['id'] == consumer_id, consumers)), None)


def get_coffee_purchases():
    purchases = requests.get(API_URL + '/purchases?limit=50').json()['purchases']
    purchases = filter(lambda x: x['product_id'] == COFFEE_PRODUCT_ID, purchases)

    for p in purchases:
        for i in range(p['amount']):
            yield {
                'name': get_consumer(p['user_id']),
                'date': datetime.fromtimestamp(mktime_tz(parsedate_tz(p['timestamp'])))
            }


to_timestamp = lambda d: d.timestamp()


def plot_it(png_name, titlestr, start_datetime=None):
    c.execute("SELECT * from coffeepot;")

    t = []
    x = []
    r = []
    for t_, s, remaining_cups, raw in c.fetchall():
        if start_datetime is not None and t_ < start_datetime:
            continue

        if x is None:
            continue

        t += [t_]
        x += [remaining_cups]
        r += [raw]

    # Data for plotting
    fig, ax = plt.subplots(figsize=(5, 7))

    # UGLY HACK: this fails due to strange reasons for the first run... 
    try:
        ax.xaxis_date()
    except ValueError:
        pass
    ax.xaxis_date()

    ax.plot(t, x)

    ax.set(xlabel='time', ylabel='Cups',
           title='Remaining Cups in Coffee Pot' + titlestr, ylim=(-0.5, 8.5))
    ax.grid()
    ax.set_yticks(range(0, 9))
    ax.legend(('Cups',), loc=2)

    texts = []
    y = [v or 0.0 for v in x]

    t_min = None

    i = 0
    for p in get_coffee_purchases():
        timestamps = list(map(to_timestamp, t))
        timestamp_now = to_timestamp(p['date'])

        new = p['date']

        if t_min is not None and new > t_min:
            new = t_min

        t_min = new - timedelta(hours=1)

        xy = (p['date'], np.interp(timestamp_now, timestamps, y))
        xy_text = (new, - 6)
        i += 1

        texts += [ax.annotate(p['name'], xy, xy_text, arrowprops=dict(arrowstyle='->', alpha=0.5, color="#00ff00"),
                              rotation=90)]

    fig.autofmt_xdate()
    fig.subplots_adjust(bottom=0.4)
    ax.xaxis.set_major_formatter(DateFormatter("%H:%M", tz=tz.gettz('Europe/Berlin')))

    fig.savefig(png_name)

    return texts


texts = plot_it('plot_fancy.png', ' (last 24h)', datetime.now() - timedelta(hours=24))
