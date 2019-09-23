#!/usr/bin/python3

import sqlite3
import requests
from datetime import datetime, date

import os

abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

BASE_URL = "http://kaffeekanne.gatrobe.de"

conn = sqlite3.connect('coffee.db', detect_types=sqlite3.PARSE_DECLTYPES)
c = conn.cursor()

# status: "unreachable", "no coffeepot", "ok"
STATUS = {
    "ok": 0,
    "no coffeepot": 1,
    "unreachable": 2,
    "timeout": 3
}

# Create table
c.execute('''CREATE TABLE IF NOT EXISTS coffeepot
             (datetime timestamp, status integer, remaining_cups real, raw_value integer)''')

time = datetime.now()

try:
    raw = None
    remaining_cups = None

    r = requests.get(BASE_URL + '/remaining_cups', timeout=5)

    if r.status_code == 200:
        status = STATUS["ok"]
        remaining_cups = float(r.text)
    elif r.status_code == 503:
        status = STATUS["no coffeepot"]
    else:
        status = STATUS["unreachable"]

    r = requests.get(BASE_URL + '/raw', timeout=5)

    if r.status_code == 200:
        raw = int(r.text)
    else:
        status = STATUS["unreachable"]

except requests.exceptions.Timeout:
    status = STATUS["timeout"]

except requests.exceptions.RequestException:
    status = STATUS["unreachable"]

c.execute("INSERT INTO coffeepot VALUES (?, ?, ?, ?)",
          (time, status, remaining_cups, raw))

# Save (commit) the changes
conn.commit()

# We can also close the connection if we are done with it.
# Just be sure any changes have been committed or they will be lost.
conn.close()
