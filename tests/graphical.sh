#!/bin/bash

rm -rf graphical
mkdir graphical

cd graphical

mkdir src
mkdir eda
mkdir ctm

cd ..

cd ../src

make clean
make graph

##########################
#        SRC 
##########################

./src_lda 	-log=true \
		 	-out=../tests/graphical \
		 	-I=500 \
		 	-in=../data/graphical/corpus.dat \
		 	-ks=../data/graphical/ks.dat \
		 	-K=10 \
		 	-model=bijective \
		 	-alg=src \
		 	-alpha=1.0 \
		 	-save=1,20,50,100,150,200,300,500 \
		 	-mu=25.0 \
		 	-sigma=0.0 | tee ../tests/graphical/output.log

##########################
#        EDA 
##########################

./src_lda 	-log=true \
		 	-out=../tests/graphical/eda \
		 	-I=500 \
		 	-in=../data/graphical/corpus.dat \
		 	-ks=../data/graphical/ks.dat \
		 	-K=10 \
		 	-model=bijective \
		 	-alg=eda \
		 	-alpha=1.0 \
		 	-save=1,20,50,100,150,200,300,500 \
		 	-mu=25.0 \
		 	-sigma=0.0 | tee ../tests/graphical/output.log

##########################
#        CTM 
##########################

./src_lda 	-log=true \
		 	-out=../tests/graphical/ctm \
		 	-I=500 \
		 	-in=../data/graphical/corpus.dat \
		 	-ks=../data/graphical/ks.dat \
		 	-K=10 \
		 	-model=bijective \
		 	-alg=ctm \
		 	-alpha=1.0 \
		 	-save=1,20,50,100,150,200,300,500 \
		 	-mu=25.0 \
		 	-sigma=0.0 | tee ../tests/graphical/output.log