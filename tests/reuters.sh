#!/bin/bash

rm -rf reuters
mkdir reuters

cd reuters

mkdir ctm
mkdir src
mkdir lda

cd ..

cd ../src

make clean
make

./src_lda 	-g \
		 	-out=../tests/reuters \
		 	-ks=../data/reuters/ks.dat \
		 	-P=7 | tee ../tests/reuters/output.log

./src_lda 	-out=../tests/reuters/ctm \
		 	-I=1000 \
		 	-C=10000 \
		 	-in=../data/reuters/input.dat \
		 	-ks=../data/reuters/ks.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=ctm \
		 	-P=7 | tee ../tests/reuters/ctm/output.log

./src_lda 	-out=../tests/reuters/src \
		 	-I=1000 \
		 	-in=../data/reuters/input.dat \
		 	-ks=../data/reuters/ks.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-P=7 \
		 	-A=25 \
		 	-gt=../tests/reuters/gt.dat \
		 	-sigma=0.3 | tee ../tests/reuters/src/output.log

./src_lda 	-out=../tests/reuters/lda \
		 	-I=1000 \
		 	-in=../data/reuters/input.dat \
		 	-K=100 \
		 	-alg=lda | tee ../tests/reuters/lda/output.log