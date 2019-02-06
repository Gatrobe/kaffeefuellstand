#!/bin/sh

cd $(dirname $0)
python3 acquire.py
python3 plot.py
python3 fancy_plot.py
cp /root/kaffeefuellstand/*.png /var/www/shop-db-frontend
cp /root/kaffeefuellstand/kaffeekanne.html /var/www/shop-db-frontend
