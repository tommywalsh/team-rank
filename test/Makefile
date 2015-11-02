.PHONY: test clean

sources := $(shell ls *cpp)
executables := $(sources:.cpp=.run)
sentinels := $(executables:.run=.passed)

CXXFLAGS := -I.. -g --std=c++0x

%.run : %.cpp ../%.hpp
	$(CXX) $(CXXFLAGS) $< $(shell ls ../$< 2>/dev/null) -o $@

%.passed : %.run
	./$< && touch $@

test: $(sentinels)

clean:
	rm -f $(executables) $(sentinels)
