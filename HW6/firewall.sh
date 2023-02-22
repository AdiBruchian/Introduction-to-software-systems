#!/bin/bash

#reads from stdin, removes whitespaces, and saves the result to PACKETS variable.
PACKETS=$(cat | tr -d "[:blank:]")

#remove comments, whitespaces, blanklines and saves the result to RULES variable
RULES=$(grep -o "^[^#].*" $1 | sed 's/#.*/ /' | sed '/^$/d' | tr -d "[:blank:]")

while read -r RULE;
do
	SRC_IP_RULE=$(echo $RULE | grep -o "src-ip=[\.0-9/]\+")
	DST_IP_RULE=$(echo $RULE | grep -o "dst-ip=[\.0-9/]\+")	
	SRC_PORT_RULE=$(echo $RULE | grep -o "src-port=[\.0-9\-]\+")
	DST_PORT_RULE=$(echo $RULE | grep -o "dst-port=[\.0-9\-]\+")
	
	FILTERED_PACKETS+=$(echo "$PACKETS" \
	| ./firewall.exe "$SRC_IP_RULE"\
	| ./firewall.exe "$DST_IP_RULE"\
	| ./firewall.exe "$SRC_PORT_RULE"\
	| ./firewall.exe "$DST_PORT_RULE")
	
	FILTERED_PACKETS+="\n"
	
done <<< "$RULES"

echo  -e "$FILTERED_PACKETS" | sed '/^$/d' | sed 's/ //g' | sort | uniq 

