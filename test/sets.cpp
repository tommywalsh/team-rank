#include "sets.hpp"

#include <cassert>

int main() {

	std::set<int> evens {0,2,4,6,8};
	std::set<int> odds {1,3,5,7,9};
	std::set<int> threes {0,3,6,9};

	assert(sets::intersection(evens, odds).empty());

	auto z6 = sets::intersection(evens, threes);
	assert(z6.size() == 2);
	assert(z6.count(0) == 1);
	assert(z6.count(6) == 1);

	auto all = sets::setUnion(evens, odds);
	assert(all.size() == 10);
	for (auto i = 0; i < 10; ++i) {
		assert(all.count(i) == 1);
	}

	auto one57 = sets::difference(odds, threes);
	assert(one57.size() == 3);
	assert(one57.count(1) == 1);
	assert(one57.count(5) == 1);
	assert(one57.count(7) == 1);
}
