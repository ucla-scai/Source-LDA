Source-LDA
============

Source-LDA: Enhancing probabilistic topic models using prior knowledge sources (ICDE 2017)

## Runtime environment setup

To run this project, you first need to install:

1. [gsl](https://www.gnu.org/software/gsl/)

2. C++ 11

## Usage

Source-LDA.

Usage:

* src_lda 
* src_lda -h | --help

* src_lda 
 + [-alg=\<name>]
 + [-model=\<name>]
 + [-in=\<file>]
 + [-out=\<file>]
 + [-P=\<number>]
 + [-K=\<number>]
 + [-ks=\<file>]
 + [-I=\<number>]
 + [-C=\<number>]
 + [-ks=\<file>]
 + [-mu=\<number>]
 + [-sigma=\<number>]
 + [-perp=\<name>]
 + [-log=\<bool>]
 + [-A=\<number>]
 + [-gt=\<file>]
 + [-key=\<file>]
		
Options:

* -h --help Show this screen (also shows with no parameters)	
* -alg=\<name> Algorithm used (src|eda|ctm|lda) [default: src]
* -model=\<name>	Model used (src|bijective|mix) [default: src]
* -in=\<file> Input corpus [default: input.dat]
* -out=\<file> Directory to save output [default: .]
* -P=\<file> Number of threads [default: 1]
* -K=\<file> Number of unlabeled topics [default: 100]
* -ks=\<file> Knowledge source [default: ks.dat]
* -I=\<number> Number of iterations [default: 1000]
* -C=\<number> Top number of n-grams to keep for CTM [default: 10000]
* -mu=\<number>	Mu value for Source-LDA							[default: 0.7]
* -sigma=\<number> Sigma value for Source-LDA [default: 0.3]
* -perp=\<name>	Perplexity calculation used (none|hein|imp|lr) [default: none]
* -log=\<bool> Display log likelihood (true|false) [default: false]
* -A=\<number> Approximation steps for Source-LDA				[default: 5]
* -gt=\<file> File containing gt point pre calculations [default: gt.dat]
* -key=\<file> Key used for classification percentage [default: key.dat]