#!/bin/bash
#
#$ -cwd
#$ -j y
#$ -S /bin/bash
#$ -N elyabe
#$ -pe mpich 16

# Redirecting standard output error to files
# named "output" and "errors"
#$ -o output_par_16.txt
#$ -e errors_par_16.txt

senhas="902ba3cda1883801594b6e1b452790cc53948fda 8cb2237d0679ca88db6464eac60da96345513964 7b151de317f2547df39e1a1ff2850a6abfa6128f 230cdfcdb6f2aa33b7acf19edaae5a216a14155f b2cdbf0601d8ae90d3cda1c978566ace86c4eac0 a045b7efa463c6ed195c644163f4168952fbd34a";
# senhas="8cb2237d0679ca88db6464eac60da96345513964";
# senhas="902ba3cda1883801594b6e1b452790cc53948fda";

for s in $senhas
do
	for (( i = 0; i < 3; i++ )) 
	do
		mpirun -np 16  --oversubscribe ./sha1  $s >> saida16.txt
	done
done
