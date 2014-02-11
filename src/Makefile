Range.o: Range.cpp Range.h
	g++ -g -c -o Range.o Range.cpp 

SizedArray.o: SizedArray.cpp SizedArray.h
	g++ -g -c -o SizedArray.o SizedArray.cpp

ParallelRandomize.o: ParallelRandomize.cpp SizedArray.h
	g++ -pthread -g -c -o ParallelRandomize.o ParallelRandomize.cpp

ParallelMergesort.o: ParallelMergesort.cpp SizedArray.h
	g++ -pthread -g -c -o ParallelMergesort.o ParallelMergesort.cpp
	
ParallelQuicksort.o: ParallelQuicksort.cpp SizedArray.h
	g++ -pthread -g -c -o ParallelQuicksort.o ParallelQuicksort.cpp
	
randomize: ParallelRandomize.o SizedArray.o Range.o
	g++ -pthread -o randomize ParallelRandomize.o SizedArray.o Range.o
	
mergesort: ParallelMergesort.o SizedArray.o Range.o
	g++ -pthread -o mergesort ParallelMergesort.o SizedArray.o Range.o
	
quicksort: ParallelQuicksort.o SizedArray.o Range.o
	g++ -pthread -o quicksort ParallelQuicksort.o SizedArray.o Range.o
