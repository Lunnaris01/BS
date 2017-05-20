#!/bin/bash
# Shows CPU information if Parameter bg is added.
if [ "$#" -ge 4 ]; then
if [ "$1" -eq "$1" ] 2> /dev/null; then
if [ "$2" -eq "$2" ] 2> /dev/null; then
if [ "$4" -eq "$4" ] && [ "$4" -gt "0" ] && [ "$4" -lt "200" ] 2>/dev/null; then
if [ "$5" = "bg" ]; then
gcc -o mandel mandel.c -lpthread
./mandel $1 $2 $4 > $3 &
pid=$!
while kill -0 $pid 2> /dev/null; do
set $(sed -n 1p /proc/stat)
ZEIT=$(($2+$3+$4+$5+$6+$7+$8+$9))
IDLE=$(($5+$6))
LASTSTART=$(($ZEIT-$IDLE))
LAST=$((100*$LASTSTART/$ZEIT))

sleep 1
set $(sed -n 1p /proc/stat)
ZEIT2=$(($2+$3+$4+$5+$6+$7+$8+$9))
IDLE2=$(($5+$6))
LASTSTART2=$(($ZEIT2-$IDLE2))
LAST2=$((100*$LASTSTART2/$ZEIT2))

CPULASTCURRENT=$(((100*($LASTSTART2-$LASTSTART))/($ZEIT2-$ZEIT)))
echo "Aktuelle CPU-Auslastung: $CPULASTCURRENT"

done
else
gcc -o mandel mandel.c -lpthread
./mandel $1 $2 $4 > $3 
fi
else
echo "Parameter 4 missing or not a reasonable number(1-200), Usage: sh mandel.sh HEIGHT WIDTH OUTPUTFILE AMOUNTOFTHREADS"
fi
else
echo "Parameter 2 missing or not a number, Usage: sh mandel.sh HEIGHT WIDTH OUTPUTFILE AMOUNTOFTHREADS"
fi
else
echo "Parameter 1 missing or not a number, Usage: sh mandel.sh HEIGHT WIDTH OUTPUTFILE AMOUNTOFTHREADS"
fi
else
echo "Please provide 2 numeric Parameters as Parameter 1 and 2, an output file as parameter 3! and the Amount of Threads as Parameter 4"
fi
