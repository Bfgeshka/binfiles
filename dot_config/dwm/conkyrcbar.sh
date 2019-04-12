#		Syntax for conky v1.10		special			▲▼ ⮀⮁  ⮃⮂  		red	#eaeaea		yellow	#9DA214		green	#8bad9d		white 	#ffffff		blue	#488B84		black	#050505

uppercase				=	false
short_units				=	true



TEXT
%{U\#ff214d58}\
%{B\#ff111111} \
${if_up eth0}%{B\#ff5e7177} %{F\#ff000000}${upspeed eth0} %{F\#ff00000}⇅ %{F\#ff000000}${downspeed eth0}%{F\#ff00000} %{B\#ff111111} ${endif}\
${if_up wlan0}%{B\#ff5e7177} %{F\#ffffffff}${upspeed wlan0} %{F\#ff00000}⇅ %{F\#ffffffff}${downspeed wlan0}%{F\#ff00000} %{B\#ff111111} ${endif}\
${if_match ${battery_percent} < 95}%{B\#ff214d58} $battery_percent% $battery_time %{B\#ff111111} ${endif}\
%{B\#ff333c43}%{F\#ffeaeaea} $swap %{B\#ff111111}%{F\#ff000000} \
%{A4:rcsh $HOME/.config/dwm/bar_volup.sh:}%{A5:rcsh $HOME/.config/dwm/bar_voldown.sh:}%{B\#ff8bad9d} ${exec amixer get Master | ag -o "[0-9]+%"} %{B\#ff111111}%{A}%{A}\
%{c}\
${if_running deadbeef-gtkui}%{B\#ff214d58} \
%{A:deadbeef --prev:}%{+u+o}%{B\#ff111111}%{F\#ff214d58} ⏪ %{F\#ffcacaca}%{B\#ff111111}%{A}\
\
%{A3:deadbeef --stop:}%{A1:deadbeef --toggle-pause:}%{A4:rcsh $HOME/.config/dwm/bar_volup.sh:}%{A5:rcsh $HOME/.config/dwm/bar_voldown.sh:}%{A2:rcsh $HOME/.dwm/bar_mute.sh:}\
%{B\#ff214d58} ${exec deadbeef --nowplaying "%a || %t [%e/%l]" 2>/dev/null} \
\
%{A}%{A}%{A}%{A}%{A}\
%{A:deadbeef --next:}%{B\#ff111111}%{F\#ff214d58} ⏩ %{F\#ff00000}%{B\#ff111111}%{A}\
%{B\#ff214d58} %{B\#ff111111}%{-u-o} \
${endif}\
%{r}\
%{B\#ff8bad9d} $mem %{B\#ff111111} \
%{B\#ff333c43}%{F\#ffeaeaea}${execi 30 $HOME/.config/dwm/weather.sh} %{B\#ff111111} \
%{B\#ff5e7177}%{F\#ff000000} $cpu% || $loadavg %{F\#ff00000}%{B\#ff111111} \
