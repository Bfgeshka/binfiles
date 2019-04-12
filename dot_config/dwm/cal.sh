#!/bin/mksh

FONT="Tewi"
FONTSIZE="10"
ACT='onstart=uncollapse;button1=exit;button3=exit'

# Colors
FG="#BFBFBF"
BG="#131314"
highlight="#BFBFBF"
highlight2="#408075"

WIDTH=160
LINES=7
XPOS=1270
YPOS=16

PIPE=/tmp/cal_pipe
rm -f "$PIPE"

# TODAY=$(expr `date +'%d'` + 0)
TODAY=$(date "+%_d ")

# generate calender
CAL=$(cal -m | sed -re "s/$TODAY/^bg($highlight2)^fg($highlight)$TODAY^fg()^bg()/")

mkfifo "$PIPE"
	( dzen2 -u -bg $BG -fg $FG -fn "${FONT}:pixelsize=${FONTSIZE}" -x $XPOS -y $YPOS -w $WIDTH -l $LINES -sa 'c' -e "$ACT" -title-name 'popup_calendar' < "$PIPE"
	rm -f "$PIPE") &

# feed the pipe
(
echo "$CAL"
sleep 10
) > "$PIPE"
