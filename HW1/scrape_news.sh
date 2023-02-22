#!/bin/bash

site="https://ynetnews.com/category/3082"

#save the site's html into a variable"
data=$(wget --no-check-certificate -O - $site 2>/dev/null)

#search for all the articles according to the given regex template
articles=$(echo "$data" |\
          grep -oP "https://(www.)?ynetnews.com/article/[0-9a-zA-Z]+" | sort | uniq)

echo "$(echo "$articles" | wc -l)" >> results.csv

#read through each article and finds the number of occurences each name
#appears in the article
while read -r line; do
	current_article=$(wget -q -O - "$@" $line 2>/dev/null)
	bengvir=$(echo "$current_article" | grep -o "Ben-Gvir" | wc -l)
	lapid=$(echo "$current_article" | grep -o "Lapid" | wc -l)
	gantz=$(echo "$current_article" | grep -o "Gantz" | wc -l)
	bibi=$(echo "$current_article" | grep -o "Netanyahu" | wc -l)
	if [[(bengvir -eq 0)&&(lapid -eq 0)&&(gantz -eq 0)&&(bibi -eq 0)]]; then
	    echo "$line, -" >> results.csv
	else	     
	    echo "$line, Netanyahu, $bibi, Gantz, $gantz, Lapid, $lapid,"\
	         " Ben-Gvir, $bengvir" >> results.csv
	fi
done <<< $articles

