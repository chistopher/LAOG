#!/bin/bash

#SBATCH --ntasks=50
#SBATCH --mem-per-cpu=3G
#SBATCH --ntasks-per-node=50
#SBATCH --output slurm.out

export CW_DIST="dist"
export CW_COST="linear"
export CW_A="1.0"
export CW_C="0.0"
export CW_START="tree"

listOfN=$(seq 11000 1000 12000)
numRuns="50"

export CW_DIR="${CW_DIST}_${CW_COST}_${CW_A}_${CW_C}_${CW_START}"
mkdir ${CW_DIR}

for n in ${listOfN}
do
	export CW_N=${n}
	srun --ntasks=${numRuns} --output="${CW_DIR}/${CW_N}_%t.log" test_multiple &
	echo "job step for n=${n} submitted"
	sleep 1
done
wait
