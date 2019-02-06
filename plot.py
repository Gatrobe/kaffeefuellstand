#!/usr/bin/python3

import sqlite3
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime, timedelta

import os

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
    
    fig.autofmt_xdate()
    
    fig.savefig(png_name)
    #plt.show()


plot_it('plot.png', '')
plot_it('plot_1h.png', ' (last 1h)', datetime.now() - timedelta(hours = 1))
plot_it('plot_6h.png', ' (last 6h)', datetime.now() - timedelta(hours = 6))
plot_it('plot_24h.png', ' (last 24h)', datetime.now() - timedelta(hours = 24))
