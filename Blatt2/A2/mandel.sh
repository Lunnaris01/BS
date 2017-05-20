#!/bin/bash
# Shows CPU information if Parameter bg is added.
# 1 - Check Amount of Arguments
if [ "$#" -ge 4 ]; then
# 2 - Check First Argument Numb
if [ "$1" -eq "$1" ] 2> /dev/null; then
# 3 - Check 2nd Argument Numb
if [ "$2" -eq "$2" ] 2> /dev/null; then
# 4 - Check 4th Argument (Threads) is reasonable Number
if [ "$4" -eq "$4" ] && [ "$4" -gt "0" ] && [ "$4" -lt "200" ] 2>/dev/null; then
#With 4 Correct Parameters the others are ignored if wrong, since they are optional. If e.g. Parameter 5 is "dsafs" the Programm will act as if no 5th Parameter was given. -> Compile here
gcc -o mandel mandel.c -lpthread

# 5 - Check if 5th Argument is reasonable Number for Benchmarking
if [ "$5" -eq "$5" ] 2>/dev/null && [ "$5" -ge "$4" ] 2>/dev/null && [ "$5" -lt "200" ] 2>/dev/null; then
# 6 - Check if bg Parameter is given for Benchmarking-Mode
if [ "$6" = "bg" ]; then
echo "Running Benchmark Mode in Background."
i=$4
max=$5
h=$1
w=$2
out=$3
while [ $i -le $max ]
do
echo "Running with $i Threads"
./mandel $h $w $i > $out &
pid=$!
while kill -0 $pid 2> /dev/null; do
set $(sed -n 1p /proc/stat)
ZEIT=$(($2+$3+$4+$5+$6+$7+$8+$9))
IDLE=$(($5+$6))
LASTSTART=$(($ZEIT-$IDLE))
LAST=$((100*$LASTSTART/$ZEIT))

sleep 0.5
set $(sed -n 1p /proc/stat)
ZEIT2=$(($2+$3+$4+$5+$6+$7+$8+$9))
IDLE2=$(($5+$6))
LASTSTART2=$(($ZEIT2-$IDLE2))
LAST2=$((100*$LASTSTART2/$ZEIT2))

CPULASTCURRENT=$(((100*($LASTSTART2-$LASTSTART))/($ZEIT2-$ZEIT)))
echo "Aktuelle CPU-Auslastung: $CPULASTCURRENT"

done
i=$((i+1))
done

# 6 - No BG-Parameter -> Benchmark without Background
else
echo "Running Benchmark Mode"
i=$4
max=$5
while [ $i -le $max ]
do
echo "Running with $i Threads"
./mandel $1 $2 $i > $3 
i=$((i+1))

done

fi
# 5 - 5th Argument not given or no Benchmarking Parameter
else
if [ "$#" -ge 5 ] && [ "$5" != "bg" ]; then
echo "5th Argument needs to be a Number bigger than 4th Argument and between 1 and 200 for Benchmarking, or needs to be bg for background mode without Benchmarking"
return
fi
echo "No Benchmarking"
# 8 - Check Background for no Benchmark
if [ "$5" = "bg" ]; then
echo "Background - No Benchmark"

./mandel $1 $2 $4 > $3 &
pid=$!
while kill -0 $pid 2> /dev/null; do
set $(sed -n 1p /proc/stat)
ZEIT=$(($2+$3+$4+$5+$6+$7+$8+$9))
IDLE=$(($5+$6))
LASTSTART=$(($ZEIT-$IDLE))
LAST=$((100*$LASTSTART/$ZEIT))

sleep 0.5
set $(sed -n 1p /proc/stat)
ZEIT2=$(($2+$3+$4+$5+$6+$7+$8+$9))
IDLE2=$(($5+$6))
LASTSTART2=$(($ZEIT2-$IDLE2))
LAST2=$((100*$LASTSTART2/$ZEIT2))

CPULASTCURRENT=$(((100*($LASTSTART2-$LASTSTART))/($ZEIT2-$ZEIT)))
echo "Aktuelle CPU-Auslastung: $CPULASTCURRENT"

done

# 8 - No Background no Benchmark
else
echo "No Background -No Benchmark"
./mandel $1 $2 $4 > $3 
fi
fi
# 4 Parameter 4 wrong
else
echo "Parameter 4 missing or not a reasonable number(1-200), Usage: sh mandel.sh HEIGHT WIDTH OUTPUTFILE AMOUNTOFTHREADS"
fi
# 3 Parameter 2 Wrong
else
echo "Parameter 2 missing or not a number, Usage: sh mandel.sh HEIGHT WIDTH OUTPUTFILE AMOUNTOFTHREADS"
fi
# 2 Parameter 1 Wrong
else
echo "Parameter 1 missing or not a number, Usage: sh mandel.sh HEIGHT WIDTH OUTPUTFILE AMOUNTOFTHREADS"
fi
# 1 Wrong amount of Parameters
else
echo "Please provide 2 numeric Parameters as Parameter 1 and 2, an output file as parameter 3! and the Amount of Threads as Parameter 4"
fi





