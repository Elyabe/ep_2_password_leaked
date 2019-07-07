#!/bin/bash
#
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -N elyabe
#$ -pe mpich 64

# Redirecting standard output error to files
# named "output" and "errors"
#$ -o output_par_64_desafio.txt
#$ -e errors_par_64_desafio.txt

senhas="902ba3cda1883801594b6e1b452790cc53948fda d27e086d60a993d203717509c46a6752daccc967";

for s in $senhas
do
	for (( i = 0; i < 3; i++ )) 
	do
		mpirun -np 64 ./sha1  $s >> saida64_desafio.txt
	done
done
