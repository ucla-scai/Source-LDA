#!/bin/bash

rm -rf integrate
mkdir integrate
cd integrate

mkdir zero_one
mkdir zero_two
mkdir zero_three
mkdir zero_four
mkdir zero_five
mkdir zero_six
mkdir zero_seven
mkdir zero_eight
mkdir zero_nine
mkdir one_zero
mkdir baseline

cd ..

cd ../src

make clean
make

##########################
#          0.1 
##########################

./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_one \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.1 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_one/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_one \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.1 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_one/output_acc.log


##########################
#          0.2 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_two \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.2 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_two/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_two \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.2 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_two/output_acc.log


##########################
#          0.3 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_three \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.3 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_three/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_three \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.3 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_three/output_acc.log

##########################
#          0.4 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_four \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.4 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_four/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_one \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.4 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_four/output_acc.log


##########################
#          0.5 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_five \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.5 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_five/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_five \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.5 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_five/output_acc.log


##########################
#          0.6 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_six \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.6 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_six/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_six \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.6 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_six/output_acc.log


##########################
#          0.7 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_seven \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.7 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_seven/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_seven \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.7 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_seven/output_acc.log


##########################
#          0.8 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_eight \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.8 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_eight/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_eight \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.8 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_eight/output_acc.log


##########################
#          0.9 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/zero_nine \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.9 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_nine/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/zero_nine \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.9 \
		 	-sigma=0.0 | tee ../tests/integrate/zero_nine/output_acc.log


##########################
#          1.0 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/one_zero \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=1.0 \
		 	-sigma=0.0 | tee ../tests/integrate/one_zero/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/one_zero \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=1.0 \
		 	-sigma=0.0 | tee ../tests/integrate/one_zero/output_acc.log

##########################
#        BASELINE 
##########################


./src_lda 	-log=false \
		 	-out=../tests/integrate/baseline \
		 	-I=1000 \
		 	-perp=hein \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-P=7 \
		 	-left=0.0 \
		 	-right=1.0 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.5 \
		 	-A=25 \
		 	-sigma=1.0 | tee ../tests/integrate/baseline/output_perp.log

./src_lda 	-log=false \
			-P=7 \
			-left=0.0 \
		 	-right=1.0 \
		 	-out=../tests/integrate/baseline \
		 	-I=1000 \
		 	-key=../data/integrate/key.dat \
		 	-in=../data/integrate/corpus.dat \
		 	-ks=../data/integrate/ks.dat \
		 	-K=100 \
		 	-model=bijective \
		 	-alg=src \
		 	-mu=0.5 \
		 	-A=25 \
		 	-sigma=1.0 | tee ../tests/integrate/baseline/output_acc.log		 	