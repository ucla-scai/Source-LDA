#!/bin/bash

rm -rf graphical
mkdir graphical

cd ../src

make clean
make graph

./src_lda 	-log=true \
		 	-out=../tests/graphical \
		 	-I=500 \
		 	-in=../data/graphical/corpus.dat \
		 	-ks=../data/graphical/ks.dat \
		 	-K=10 \
		 	-model=bijective \
		 	-alg=src \
		 	-save=1,20,50,100,150,200,300,500 \
		 	-mu=25.0 \
		 	-sigma=0.0 | tee ../tests/graphical/output.log