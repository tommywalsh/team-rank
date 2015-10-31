#include "poset.hpp"

#include <cassert>

int main() {
	Poset<int> poset;

	poset.addRelation(1, 5);
	poset.addRelation(1, 4);

	auto tl = poset.getTopLevel();
	assert(tl.size() == 1);
	assert(*(tl.begin()) == 1);

	assert(poset.hasRelation(1,5));
	assert(poset.hasRelation(1,4));
	assert(!poset.hasRelation(1,7));
	assert(!poset.hasRelation(5,1));

	poset.addRelation(4,7);
	tl = poset.getTopLevel();
	assert(tl.size() == 1);
	assert(poset.hasRelation(4,7));

	// Check transitivity
	assert(poset.hasRelation(1,7));

	// Check removal
	poset.removeObject(4);

	// Transitive relations should not have been dropped!
	assert(poset.hasRelation(1,7));

	// Object should have been deleted really!
	assert(!poset.hasRelation(1,4));
	assert(!poset.hasRelation(4,7));

	auto above7 = poset.getAbove(7);
	assert(above7.size() == 1);
	assert(*(above7.begin()) == 1);

	auto below1 = poset.getBelow(1);
	assert(below1.size() == 2);
	assert(below1.count(5) == 1);
	assert(below1.count(7) == 1);

}
