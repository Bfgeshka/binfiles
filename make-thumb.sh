#!/bin/sh
# Thumbnails for something, invocation from workingdir
make_thumbnail() {
    pic=$1
    thumb=$(dirname "$1")/thumb-$(basename "$1")
    convert "$pic" -thumbnail x150 "$thumb"
}

for pic in ./*.png
do
    make_thumbnail "$pic"
done
