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


for (( a = 100 ; a >= 98 ; a--  ));
    do
		if [[ $a == 100 ]];
			then 
				echo -n "$a bottles of beer on the wall, $a bottles of beer. If one of those bottles would happen to fall, ";
		elif [[ $a == 99 ]];
			then 
				echo -n "there'd be $a bottles of beer on the wall $a bottles Of beer on the wall, $a bottles Of beer. If one Of those bottles would happen to fall, ";
		elif [[ $a == 98 ]];
			then 
				echo "there'd be $a bottles of beer on the wall One bottle Of beer on the wall, I bottle Of beer. If that bottle would happen to fall, there'd be no bottles of beer on the wall.";
	fi
done
read