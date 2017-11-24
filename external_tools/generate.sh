#! /bin/sh
rm -r /home/t400/array32.bitmap
icon_path="/home/t400/Documents/icons/*.c"
for icon in $icon_path
do
    ./array8to32.py $icon
done
