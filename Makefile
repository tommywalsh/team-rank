.PHONY: all clean unittests

bin_dir := bin
build_dir := build
test_src_dir := test
test_bin_dir := testrun

main := main.cpp
sources := $(wildcard *cpp)

executable := $(bin_dir)/podrank

CXXFLAGS := -g --std=c++0x
DEPFLAGS := -MM -MMD -MP
TESTFLAGS := -I.

# Anything except our main() function goes into a library,
# to facilitate easy unit testing
lib_sources := $(filter-out $(main), $(sources))
lib_objects := $(addprefix $(build_dir)/, $(lib_sources:.cpp=.o))
library := $(bin_dir)/podrank.a

# Each .cpp file in the test directory makes its own executable unit test
test_sources := $(wildcard $(test_src_dir)/*.cpp)
test_executables := $(addprefix $(test_bin_dir)/, $(test_sources:test/%.cpp=%))
test_tokens := $(test_executables:%=%.passed)

# We autogenerate dependencies so we can automatically recomplie when headers change
lib_deps := $(addprefix $(build_dir)/, $(lib_sources:.cpp=.d))
test_deps := $(addprefix $(test_bin_dir)/, $(test_sources:test/%.cpp=%.d))
deps := $(lib_deps) $(test_deps)

all: $(executable) unittests

# The main executable goes into the bin directory
$(executable) : $(main) $(library)
	@mkdir -p $(bin_dir)
	$(CXX) $(CXXFLAGS) $^ -o $@

# All object files go into the build directory
$(build_dir)/%.o : %.cpp
	@mkdir -p $(build_dir)
	$(CXX) $(CXXFLAGS) $< -c -o $@

# Store dependencies in the build directory as well
$(build_dir)/%.d : %.cpp
	@mkdir -p $(build_dir)
	$(CXX) $(DEPFLAGS) -MT $(@:%d=%o) $< -o $@

# All non-main files get compiled into a static library in the build dir
$(library) : $(lib_objects)
	@mkdir -p $(bin_dir)
	$(AR) $(ARFLAGS) $@ $^

# Store unit test dependencies in the tests' binary dir
$(test_bin_dir)/%.d : $(test_src_dir)/%.cpp
	@mkdir -p $(test_bin_dir)
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) $(DEPFLAGS) -MT $(@:%d=%o) $< -o $@

# Each test file makes its own executable in the tests' binary dir
$(test_bin_dir)/% : $(test_src_dir)/%.cpp $(library)
	@mkdir -p $(test_bin_dir)
	$(CXX) $(CXXFLAGS) $(TESTFLAGS) $^ -o $@

# This runs a unit test and, on success, leaves behind a token file
$(test_bin_dir)/%.passed : $(test_bin_dir)/%
	$< && touch $@

unittests: $(test_tokens)

clean :
	rm -rf $(bin_dir) $(build_dir) $(test_bin_dir) $(deps)

-include $(deps)
