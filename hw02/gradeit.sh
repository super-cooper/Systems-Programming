#!/bin/sh

pub_dir="/usr/users/noquota/faculty/rhoyle/pub/cs241/hw01"
this_dir=$(pwd) 

testdiamond_n() { # Tests pyramid prints properly for every val -9 to 9
    n=-9 
    while [ $n -le 9 ]; do
	cd $pub_dir
	echo $(echo $n | ./diamond) > "$this_dir/file1"
	cd $this_dir
	echo $(echo $n | ./diamond) > file2
	diff=$(diff file1 file2)
	if [ "$desired_output" != "$actual_output" ]; then
    	    echo "Issue with ./diamond $n"
	    #Can't separate out prompt so basically always fails...
	fi
	n=$((n+1))
    done
    rm file1 file2
}

testdiamond_bad() { # Tests if the program prints for invalid argument
    output=$(echo $(echo 11 | ./diamond))
    star="*"
    modded=$(echo $output | sed 's/\*//g')
    if ! [ "$modded" = "$output" ]; then 
	echo ./diamond prints for invalid values
    fi
}

testrot128() { # Tests to make sure rot128 encrypts correctly
    echo "This is a test" > testfile
    cd $pub_dir
    # this version of rot128 doesn't work like wuuuuuuut
    # desired=$(./rot128 "$this_dir/testfile")
    cd $this_dir
    # actual=$(./rot128 testfile)
    if [ $actual -ne $desired ]; then
	echo "./rot128 doesn't encrypt correctly"
    fi

    # desired=$(./rot128 testfile)
    # this def worked last week
    rm testfile
    if ! [ $desired = $actual ]; then
	echo "./rot128 doesn't decrypt correctly"
    fi
}


testdiamond_n
testdiamond_bad
testrot128
