#!/usr/bin/python3

import sqlite3
import matplotlib.pyplot as plt
import numpy as np

conn = sqlite3.connect('coffee.db', detect_types=sqlite3.PARSE_DECLTYPES)
c = conn.cursor()

# status: "unreachable", "no coffeepot", "ok"
STATUS = {
        "ok": 0,
        "no coffeepot": 1,
        "unreachable": 2
}

c.execute("SELECT * from coffeepot;")

t = []
x = []
r = []
for datetime, s, remaining_cups, raw in c.fetchall():
    t += [datetime]
    x += [remaining_cups]
    r += [raw]

# Data for plotting
fig, ax = plt.subplots()
ax.plot(t, x)

ax.set(xlabel='time', ylabel='Cups',
       title='Remaining Cups in Coffee Pot', ylim=(-0.5,8.5))
ax.grid()
ax.legend(('Cups',), loc=2)


ax2 = ax.twinx()
ax2.plot(t, r, 'r')
ax2.set(ylim=(0, 1024), ylabel="Raw ADC Value")
ax2.legend(('Raw',), loc=1)


fig.savefig("plot.png")
#plt.show()

