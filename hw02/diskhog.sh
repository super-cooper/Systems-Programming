#/!bin/sh

# Lists the top 5 heaviest disk users in the current directory

dir=$(ls)
top=$(du -a $dir | sort -n -r | head -n 5)

for item in $top; do
    if [ -f $item ] || [ -d $item ]; then
	du -d 0 -h $item 
    fi
done
