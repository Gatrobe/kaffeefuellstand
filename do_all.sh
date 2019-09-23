#!/bin/sh

cd $(dirname $0)
venv/bin/python3 acquire.py
venv/bin/python3 plot.py
venv/bin/python3 fancy_plot.py
cp *.png /var/www/kaffeefuellstand
cp kaffeekanne.html /var/www/kaffeefuellstand
