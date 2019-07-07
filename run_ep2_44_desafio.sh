#!/bin/bash
#
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -N elyabe
#$ -pe mpich 44

# Redirecting standard output error to files
# named "output" and "errors"
#$ -o output_par_44_desafio.txt
#$ -e errors_par_44_desafio.txt

senhas="d27e086d60a993d203717509c46a6752daccc967";

for s in $senhas
do
	for (( i = 0; i < 3; i++ )) 
	do
		mpirun -np 44 ./sha1  $s >> saida44_desafio.txt
	done
done
