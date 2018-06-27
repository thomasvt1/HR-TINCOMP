#!/bin/bash
naam="Bob"
echo -n "Hallo $naam het is nu: "
date +"%c"

aantal=`wc -l gisteren.txt | awk '{print $1}'`
printf "Het bestand gisteren bevat $aantal namen"
echo ""

geslacht=$1

if [[ $geslacht == man ]];
then 
    echo "Hallo meneer!";
else
    echo "Hallo Mevrouw!";
fi


for (( a = 100 ; a >= 1 ; a--  ));
    do
		if [[ $a > 1 ]];
			then
				echo "$a bottles of beer on the wall, $a bottles of beer. If one of those bottles would happen to fall, there'd be $(( $a-1 )) bottles of beer on the wall."
		else
			echo "One bottle Of beer on the wall, 1 bottle Of beer. If that bottle would happen to fall, there'd be no bottles of beer on the wall.";
	fi
done
read