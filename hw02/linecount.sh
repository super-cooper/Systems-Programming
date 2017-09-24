#!/bin/sh

# Counts all the lines of specified files

findlines() {
    line_count=0
    for regex in $@; do
	files=$(find $regex)
	for file in $files; do
	    lines=$(wc -l $file | sed 's/[^0-9]*//g') # Remove file name
	    line_count=$(expr $line_count + $lines)
	done
    done
    echo $line_count
}

if [ $# -lt 1 ]; then
    findlines * # find all
else
    findlines $@ # find specified files
fi
