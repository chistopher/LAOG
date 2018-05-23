#!/bin/bash

listOfN=$(seq 1000 500 1500)

dist="two"
cost="linear"
a="0.46"
c="0.0"
start="circle"

numRuns="2"

function run {
    echo starting $2
    eval $1 2> "${2}.log"
    echo finished ${2}
}


dirname="${dist}_${cost}_${a}_${c}_${start}"
mkdir ${dirname}

for n in ${listOfN}
do
    for i in `seq 1 ${numRuns}`
    do
        filename="${dirname}/${n}_${i}"
        seed=$((i + n))
        run "${1} -n ${n} -dist ${dist} -cost ${cost} -a ${a} -c ${c} -start ${start} -seed ${seed} -file ${filename}" ${filename} &
    done
done
wait
