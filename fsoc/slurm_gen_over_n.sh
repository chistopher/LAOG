#!/bin/bash

export CW_DIST="two"
export CW_COST="linear"
export CW_A="0.46"
export CW_C="0.0"
export CW_START="circle"

listOfN=$(seq 1000 500 1500)
numRuns="1"

export CW_DIR="${CW_DIST}_${CW_COST}_${CW_A}_${CW_C}_${CW_START}"
mkdir ${CW_DIR}

for n in ${listOfN}
do
    for i in `seq 1 ${numRuns}`
    do
        export CW_N=${n}
        export CW_FILE="${n}_${i}"
        export CW_SEED=$((i + n))
        sbatch -J ${CW_DIR}_${CW_FILE} -o ${CW_DIR}/${CW_FILE}.log conf.slurm
    done
done
wait
