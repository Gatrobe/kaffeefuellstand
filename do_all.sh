#!/bin/sh

cd $(dirname $0)
python3 acquire.py
python3 plot.py
python3 fancy_plot.py
cp *.png /var/www/kaffeefuellstand
cp kaffeekanne.html /var/www/kaffeefuellstand
