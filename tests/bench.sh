#!/bin/bash

rm -rf bench
mkdir bench

cd bench
mkdir 1
mkdir 3
mkdir 6

cd 1
mkdir fifty
mkdir one_hundred
mkdir five_seventy_eight
mkdir nine_hundred
mkdir twenty_four_hundred
mkdir fourty_nine_hundred
mkdir seventy_four_hundred
mkdir ninety_nine_hundred
cd ..

cd 3
mkdir fifty
mkdir one_hundred
mkdir five_seventy_eight
mkdir nine_hundred
mkdir twenty_four_hundred
mkdir fourty_nine_hundred
mkdir seventy_four_hundred
mkdir ninety_nine_hundred
cd ..

cd 6
mkdir fifty
mkdir one_hundred
mkdir five_seventy_eight
mkdir nine_hundred
mkdir twenty_four_hundred
mkdir fourty_nine_hundred
mkdir seventy_four_hundred
mkdir ninety_nine_hundred
cd ..


cd ..

cd ../src

make clean
make

##########################
#        50 
##########################

./src_lda 	-log=false \
		 	-out=../tests/bench/1/fifty \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_50.dat \
		 	-K=50 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=1 \
		 	-sigma=0.3 | tee ../tests/bench/1/fifty/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/3/fifty \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_50.dat \
		 	-K=50 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=3 \
		 	-sigma=0.3 | tee ../tests/bench/3/fifty/output.log		 	

./src_lda 	-log=false \
		 	-out=../tests/bench/6/fifty \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_50.dat \
		 	-K=50 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=6 \
		 	-sigma=0.3 | tee ../tests/bench/6/fifty/output.log


##########################
#        100 
##########################

./src_lda 	-log=false \
		 	-out=../tests/bench/1/one_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_100.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=1 \
		 	-sigma=0.3 | tee ../tests/bench/1/one_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/3/one_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_100.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=3 \
		 	-sigma=0.3 | tee ../tests/bench/3/one_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/6/one_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_100.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=6 \
		 	-sigma=0.3 | tee ../tests/bench/6/one_hundred/output.log		 			 	

##########################
#        578 
##########################

./src_lda 	-log=false \
		 	-out=../tests/bench/1/five_seventy_eight \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_578.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=1 \
		 	-sigma=0.3 | tee ../tests/bench/1/five_seventy_eight/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/3/five_seventy_eight \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_578.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=3 \
		 	-sigma=0.3 | tee ../tests/bench/3/five_seventy_eight/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/6/five_seventy_eight \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_578.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=6 \
		 	-sigma=0.3 | tee ../tests/bench/6/five_seventy_eight/output.log

##########################
#        900 
##########################

./src_lda 	-log=false \
		 	-out=../tests/bench/1/nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=1 \
		 	-sigma=0.3 | tee ../tests/bench/1/nine_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/3/nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=3 \
		 	-sigma=0.3 | tee ../tests/bench/3/nine_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/6/nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=6 \
		 	-sigma=0.3 | tee ../tests/bench/6/nine_hundred/output.log


##########################
#        2400 
##########################

./src_lda 	-log=false \
		 	-out=../tests/bench/twenty_four_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_2400.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-sigma=0.3 | tee ../tests/bench/twenty_four_hundred/output.log

##########################
#        4900 
##########################

./src_lda 	-log=false \
		 	-out=../tests/bench/1/fourty_nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_4900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=1 \
		 	-sigma=0.3 | tee ../tests/bench/1/fourty_nine_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/3/fourty_nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_4900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=3 \
		 	-sigma=0.3 | tee ../tests/bench/3/fourty_nine_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/6/fourty_nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_4900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=6 \
		 	-sigma=0.3 | tee ../tests/bench/6/fourty_nine_hundred/output.log		 			 	

##########################
#        7400 
##########################

./src_lda 	-log=false \
		 	-out=../tests/bench/1/seventy_four_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_7400.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=1 \
		 	-sigma=0.3 | tee ../tests/bench/1/seventy_four_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/3/seventy_four_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_7400.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=3 \
		 	-sigma=0.3 | tee ../tests/bench/3/seventy_four_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/6/seventy_four_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_7400.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=6 \
		 	-sigma=0.3 | tee ../tests/bench/6/seventy_four_hundred/output.log		 	

##########################
#        9900 
##########################

./src_lda 	-log=false \
		 	-out=../tests/bench/1/ninety_nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_9900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=1 \
		 	-sigma=0.3 | tee ../tests/bench/1/ninety_nine_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/3/ninety_nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_9900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=3 \
		 	-sigma=0.3 | tee ../tests/bench/3/ninety_nine_hundred/output.log

./src_lda 	-log=false \
		 	-out=../tests/bench/6/ninety_nine_hundred \
		 	-I=10 \
		 	-in=../data/bench/input.dat \
		 	-ks=../data/bench/ks_9900.dat \
		 	-K=100 \
		 	-model=src \
		 	-alg=src \
		 	-mu=0.7 \
		 	-A=10 \
		 	-P=6 \
		 	-sigma=0.3 | tee ../tests/bench/6/ninety_nine_hundred/output.log		 			 	