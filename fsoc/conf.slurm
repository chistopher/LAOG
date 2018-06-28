#!/bin/bash

#SBATCH --ntasks=1
#SBATCH --export=ALL
#SBATCH --workdir=.

srun ./laogen -n ${CW_N} -dist ${CW_DIST} -cost ${CW_COST} -a ${CW_A} -c ${CW_C} -start ${CW_START} -seed ${CW_SEED} -file ${CW_DIR}/${CW_FILE}
