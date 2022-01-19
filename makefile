CXX = g++
CXXFLAGS = -Wall

test: swarm.o mytest.cpp
	$(CXX) $(CXXFLAGS) swarm.o mytest.cpp -o test

driver: swarm.o driver.cpp
	$(CXX) $(CXXFLAGS) swarm.o driver.cpp -o driver

swarm.o: swarm.h swarm.cpp
	$(CXX) $(CXXFLAGS) -c swarm.cpp

run:
	./test

run2:
	./driver
	
val:
	valgrind ./test

val2:
	valgrind ./driver

clean:
	rm *~
	rm *.o