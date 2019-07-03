#!/bin/bash
#
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -N elyabe
#$ -pe mpich 16

# Redirecting standard output error to files
# named "output" and "errors"
#$ -o output_par.txt
#$ -e errors_par.txt

mpirun -np 8 --oversubscribe ./sha1  8cb2237d0679ca88db6464eac60da96345513964