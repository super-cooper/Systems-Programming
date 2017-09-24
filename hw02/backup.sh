#!/bin/sh

# Backs up files to a specified directory

backup_dir=$1
file_count=2

if [ $# -lt 2 ]; then # Usage error
    echo "Usage: ./backup <dir> <files...>"
    exit 1
fi

if ! [ -d $backup_dir ]; then # If dir doesn't exist 
    mkdir $backup_dir # make dir
fi

count=1
for file in $@; do # for each file specified
    if [ $count -lt $file_count ]; then # This gets us to the file args
	count=$((count+1))
	continue
    fi

    if ! [ -f $file ]; then # If file DNE
	echo "Not found: $file"
	continue
    fi

    if ! [ -f "$backup_dir/$file" ]; then # If file hasn't been backed up
	cp $file $backup_dir # Back up file
	continue
    fi
    
    file_time=$(date -r $file +%s)
    last_backed_up=$(date -r "$backup_dir/$file" +%s)

    if [ $file_time -gt $last_backed_up ]; then #if changed since last backup
	rm "$backup_dir/$file" # Create new backup
	cp $file $backup_dir
    fi
done


# We assume this is what the extra credit wanted us to do
n=0
while [ $n -lt 5 ]; do
    symlink="backup$n"
    dest=$(readlink -f $symlink)
    if [ $dest = $backup_dir ]; then # Check if symlink exists for this dir
	exit 0
    fi

    if ! [ -d $symlink ]; then # If space in symlink list, add a new one
	ln -s $backup_dir $symlink
	exit 0
    fi 
    n=$(($n+1))
done # All symlinks taken if we get this far

old_dest=$backup_dir
n=0
while [ $n -lt 5 ]; do # Move all symlinks over
    symlink="backup$n"
    temp=$(readlink -f $symlink)
    rm $symlink
    ln -s $old_dest $symlink
    old_dest=$temp
    n=$(($n+1))
done

exit 0
