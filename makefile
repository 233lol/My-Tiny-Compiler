CXX=clang++
ifeq ($(CXX),clang++)
	CXXFLAGS=-fno-limit-debug-info -Wall -g -pipe 
else
	CXXFLAGS=-Wall -g -pipe 
endif
main:fio.o LR1.o LALR1.o gramma.o main.o mindfa.o mindfa2lex.o nfatodfa.o regexlex.o regextonfa.o medto4.o DAG.o
	$(CXX) $(CXXFLAGS)  $^ -o $@.exe

$./%.o:%.cpp
	$(CXX) -c $< -o $@ 
clean:
	del *.o