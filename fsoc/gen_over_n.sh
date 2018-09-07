#!/bin/bash

#SBATCH --ntasks 800
#SBATCH --mem-per-cpu=4G
#SBATCH --ntasks-per-node=50
#SBATCH --output slurm.out

export CW_DIST="dist"
export CW_COST="linear"
export CW_A="1.0"
export CW_C="0.0"
export CW_START="tree"
export CW_GREEDY="0"

listOfN=$(seq 1000 1000 100000)
numRuns="50"

export CW_DIR="${CW_DIST}_${CW_COST}_${CW_A}_${CW_C}_${CW_START}"
mkdir ${CW_DIR}

for n in ${listOfN}
do
	export CW_N=${n}
	srun --ntasks=${numRuns} --output="${CW_DIR}/${CW_N}_%t.log" test_multiple &
	echo "job step for n=${n} submitted"
	sleep 3
done
wait
