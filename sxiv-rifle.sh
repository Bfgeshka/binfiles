#!/bin/sh

SRCFILE=/mnt/tmp/sxiv_r
rm -f $SRCFILE

[ "$1" == '--' ] && shift

find -L "$(dirname "$1")" -maxdepth 1 -type f -iregex '.*\(jpe?g\|bmp\|png\|gif\)$' -print0 > $SRCFILE
sort -z -o $SRCFILE $SRCFILE

count="$(grep -zn "$1" $SRCFILE | cut -d: -f1)"

xargs -0 -a $SRCFILE sxiv -a -n $count --

exit 0
