#!/bin/bash

rm -rf wiki
mkdir wiki

cd wiki

mkdir src
mkdir eda
mkdir ctm
mkdir lda

cd src
mkdir bijective
mkdir src

cd ..

cd eda
mkdir bijective
mkdir src

cd ..

cd ctm
mkdir bijective
mkdir src

cd ..

cd ..

cd ../src

make clean
make

##########################
#        SRC 
##########################

./src_lda 	-out=../tests/wiki/src/bijective \
		 	-I=1000 \
		 	-in=../data/wiki/input.dat \
		 	-ks=../data/wiki/ks_bijective.dat \
		 	-K=100 \
		 	-key=../data/wiki/key.dat \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=5.0 \
		 	-P=7 \
		 	-A=10 \
		 	-sigma=2.0 | tee ../tests/wiki/src/bijective/output.log

./src_lda 	-out=../tests/wiki/src/src \
		 	-I=1000 \
		 	-in=../data/wiki/input.dat \
		 	-ks=../data/wiki/ks_src.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-key=../data/wiki/key.dat \
		 	-mu=5.0 \
		 	-P=7 \
		 	-A=10 \
		 	-sigma=2.0 | tee ../tests/wiki/src/src/output.log

##########################
#        EDA 
##########################

./src_lda 	-out=../tests/wiki/eda/bijective \
		 	-I=1000 \
		 	-in=../data/wiki/input.dat \
		 	-ks=../data/wiki/ks_bijective.dat \
		 	-K=100 \
		 	-key=../data/wiki/key.dat \
		 	-model=bijective \
		 	-alg=eda \
		 	-P=7 | tee ../tests/wiki/eda/bijective/output.log

./src_lda 	-out=../tests/wiki/eda/src \
		 	-I=1000 \
		 	-in=../data/wiki/input.dat \
		 	-ks=../data/wiki/ks_src.dat \
		 	-K=100 \
		 	-key=../data/wiki/key.dat \
		 	-model=src \
		 	-alg=eda \
		 	-P=7 | tee ../tests/wiki/eda/src/output.log

##########################
#        CTM 
##########################

./src_lda 	-out=../tests/wiki/ctm/bijective \
		 	-I=1000 \
		 	-in=../data/wiki/input.dat \
		 	-ks=../data/wiki/ks_bijective.dat \
		 	-K=100 \
		 	-C=10000 \
		 	-key=../data/wiki/key.dat \
		 	-model=bijective \
		 	-alg=ctm \
		 	-P=7 | tee ../tests/wiki/ctm/bijective/output.log

./src_lda 	-out=../tests/wiki/ctm/src \
		 	-I=1000 \
		 	-in=../data/wiki/input.dat \
		 	-ks=../data/wiki/ks_src.dat \
		 	-K=100 \
		 	-C=10000 \
		 	-key=../data/wiki/key.dat \
		 	-model=src \
		 	-alg=ctm \
		 	-P=7 | tee ../tests/wiki/ctm/src/output.log

##########################
#        LDA 
##########################

./src_lda 	-out=../tests/wiki/lda \
		 	-I=1000 \
		 	-alg=lda \
		 	-in=../data/wiki/input.dat \
		 	-K=100 | tee ../tests/wiki/lda/output.log