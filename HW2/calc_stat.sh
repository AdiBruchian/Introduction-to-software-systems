#!/bin/bash


#argument check
if [[ $# -eq 0 ]] ; then
    echo "Wrong number of arguments"
    exit 1
fi

course_num=$1
data=$(cat "$1.txt")
num_lines=$(cat $1.txt | wc -l)

#check if course exists in dir
if [[ ! -f  "$course_num.txt" ]]
then
    echo "Course not found"
    exit 1
fi

course_dir=$course_num"_stat"

#check if dir already exists and delete it if it is
if [[ -d $course_dir ]]
then
    rm -r $course_dir
fi

mkdir $course_dir

#execute all c files
gcc -g -Wall hist.c -o hist.exe
gcc -g -Wall min.c -o min.exe
gcc -g -Wall max.c -o max.exe
gcc -g -Wall mean.c -o mean.exe
gcc -g -Wall median.c -o median.exe

#create histogram with hist function
echo "$data" | ./hist.exe > ./$course_dir/histogram.txt

#run functions for data
min=$(echo "$data" | ./min.exe)
max=$(echo "$data" | ./max.exe)
median=$(echo "$data" | ./median.exe)
mean=$(echo "$data" | ./mean.exe)

#percent calculation
passers_hist=$(echo "$data" | ./hist.exe -n_bins 20)
passers=0

while read -r line; do
	temp_read=$(echo $line | awk '{if ( $1>54 ) {print $2}}' )
	passers=$((passers+temp_read))
done <<< $passers_hist

pass_percent="$(((($passers)*(100))/($num_lines)))%"

echo -e "$mean\t$median\t$min\t$max\t$pass_percent" > ./$course_dir/statistics.txt

