#! /bin/bash

find /home/bfg/music/ -regextype posix-egrep -regex ".*\.(jpe?g|png|gif|jpeg|tif|tiff|bmp)$" -exec rm -f {} +
find /home/bfg/music/ -regextype posix-egrep -regex ".*\.(JPE?G|PNG|GIF|JPEG|TIF|TIFF|BMP)$" -exec rm -f {} +
find /home/bfg/music/ -regextype posix-egrep -regex ".*\.(m3u8?|cue)$" -exec rm -f {} +
find /home/bfg/music/ -regextype posix-egrep -regex ".*\.(log|txt|pdf|nfo|ini)$" -exec rm -f {} +
# find /home/bfg/music/ -name "*.m3u" -exec rm -f {} +
# find /home/bfg/music/ -name "*.m3u8" -exec rm -f {} +
# find /home/bfg/music/ -name "*.log" -exec rm -f {} +
# find /home/bfg/music/ -name "*.pdf" -exec rm -f {} +
# find /home/bfg/music/ -name "*.txt" -exec rm -f {} +
# find /home/bfg/music/ -name "*.nfo" -exec rm -f {} +
find /home/bfg/music/ -name "*.sfv" -exec rm -f {} +
find /home/bfg/music/ -name "*.accurip" -exec rm -f {} +
find /home/bfg/music/ -name "Thumbs.db" -exec rm -f {} +
# find /home/bfg/music/ -name "*.cue" -exec rm -f {} +
exit 0
