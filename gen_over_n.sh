#!/bin/bash

listOfN=$(seq 1000 1000 10000)

dist="dist"
cost="linear"
a="1"
c="0"
start="tree"
greedy="0"

numRuns="5"

function run {
    echo starting $2
    date >> "${2}.log"
    eval $1 2>> "${2}.log"
    date >> "${2}.log"
    echo finished ${2}
}


dirname="${dist}_${cost}_${a}_${c}_${start}"
mkdir ${dirname}

for n in ${listOfN}
do
    for i in `seq 0 $((numRuns-1))`
    do
        filename="${dirname}/${n}_${i}"
        seed=$((i + n))
        run "${1} -n ${n} -dist ${dist} -cost ${cost} -a ${a} -c ${c} -start ${start} -greedy ${greedy} -seed ${seed} -file ${filename}" ${filename} &
    done
done
wait
