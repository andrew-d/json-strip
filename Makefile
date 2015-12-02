CPPFLAGS := -Wall -Wextra
CXXFLAGS := -std=c++11
LDFLAGS :=


all: strip

strip: strip.o main.o
	$(CXX) -o $@ $^ $(LDFLAGS)

strip.o: strip.cpp strip.hpp
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)

main.o: main.cpp strip.hpp
	$(CXX) -c -o $@ $< $(CPPFLAGS) $(CXXFLAGS)
