#! /bin/sh
rm -r ./array32.bitmap
icon_path="$1*.c"
for icon in $icon_path
do
    ./array8to32.py $icon
done
