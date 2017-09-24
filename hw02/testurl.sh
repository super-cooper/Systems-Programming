#!/bin/sh
# Usage: testurl.sh <URL.txt> 
# Reads a file containing URLs line by line, testing if the website is up


input=$1
if [ $# = 0 ]; then
    echo "Usage: ./testurl.sh <URL.txt>"
    exit 1
fi
    
while read -r line; do
    end=$(curl -s $line | tail -n 1)
    if [ -z "$end" ]; then
	echo "Not found: $line"
    fi
done < $input
