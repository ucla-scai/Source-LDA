all:
	g++ -std=c++11 main.cpp srclda.cpp srclda_parallel.cpp gtpoints.cpp -lgsl -lm -lgslcblas -lpthread -o src_lda

graph:
	g++ -std=c++11 -DEPS=0.1 main.cpp srclda.cpp srclda_parallel.cpp gtpoints.cpp -lgsl -lm -lgslcblas -lpthread -o src_lda

clean:
	-rm -f src_lda