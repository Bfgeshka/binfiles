#!/bin/sh

if ps -C cmus > /dev/null; then

	outp="`cmus-remote -Q`"
	if echo "$outp" | ag "status" | ag -v "stopped" > /dev/null; then
		artist=`echo "$outp" | ag "tag artist" | sed -e 's/tag\ artist\ //'`
		title=`echo "$outp" | ag "tag title" | sed -e 's/tag\ title\ //'`
		elapsed=`cmus-remote -Q | awk '$1 == "position" {printf("%d:%02d", $2/60, $2%60);}'`
		total=`cmus-remote -Q | awk '$1 == "duration" {printf("%d:%02d", $2/60, $2%60);}'`
		echo "$artist || $title [$elapsed/$total]"
	fi
fi
