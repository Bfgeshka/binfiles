#!/bin/bash

destpath=~/I/test/lt64kjpg
sourcepath=~/I/random/lt64kjpg
num=150

cd $sourcepath
Z=$(ls | wc -l)
dnum=1
until [ $Z -eq 0 ]
do
	mkdir -p $destpath/"$dnum"
		for x in $(find -type f | head -n$num)
		do
		mv $x $destpath/"$dnum"
		done
let dnum=$dnum+1
Z=$(ls | wc -l)
done
exit 0
