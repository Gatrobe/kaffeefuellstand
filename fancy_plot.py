#!/usr/bin/python3

import sqlite3
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import requests
from datetime import datetime, timedelta

import os

API_URL = 'http://shop.gatrobe.de/api'
COFFEE_PRODUCT_ID = 1

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
        consumers = requests.get(API_URL + '/consumers').json()

    return next(map(lambda x: x['name'], filter(lambda x: x['id'] == consumer_id, consumers)), None)

def get_coffee_purchases():
    purchases = requests.get(API_URL + '/purchases/50').json()
    purchases = filter(lambda x: x['product_id'] == COFFEE_PRODUCT_ID, purchases)

    for p in purchases:
        if p['revoked']:
            continue
        for i in range(p['amount']):
            yield {
                'name': get_consumer(p['consumer_id']),
                'date': datetime.strptime(p['timestamp'], '%a, %d %b %Y %X %Z')
            }

to_timestamp = lambda d: d.timestamp()

def plot_it(png_name, titlestr, start_datetime = None):
    c.execute("SELECT * from coffeepot;")
    
    t = []
    x = []
    r = []
    for t_, s, remaining_cups, raw in c.fetchall():
        if start_datetime is not None and t_ < start_datetime:
             continue
        t += [t_]
        x += [remaining_cups]
        r += [raw]
    
    # Data for plotting
    fig, ax = plt.subplots()
    ax.plot(t, x)
    
    ax.set(xlabel='time', ylabel='Cups',
           title='Remaining Cups in Coffee Pot' + titlestr, ylim=(-0.5,8.5))
    ax.grid()
    ax.set_yticks(range(0,9))
    ax.legend(('Cups',), loc=2)
    
    
    ax2 = ax.twinx()
    ax2.plot(t, r, 'r')
    ax2.set(ylim=(0, 1024), ylabel="Raw ADC Value")
    ax2.legend(('Raw',), loc=1)

    y = [v or 0.0 for v in x]

    for p in get_coffee_purchases():
        timestamps = list(map(to_timestamp, t))
        timestamp_now = to_timestamp(p['date'])

        xy = (p['date'], np.interp(timestamp_now, timestamps, y))
        xy_text = (p['date'], np.interp(timestamp_now, timestamps, y)+0.5)

        ax.annotate(p['name'], xy, xy_text, arrowprops=dict(arrowstyle='->'))
    
    fig.autofmt_xdate()
    
    fig.savefig(png_name)
    #plt.show()


plot_it('plot_fancy.png', ' (last 6h)', datetime.now() - timedelta(hours = 6))