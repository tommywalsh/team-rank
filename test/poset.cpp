#include "poset.hpp"

#include <cassert>


void checkGrandRelations() {
	Poset<int> poset;

	// Set up 1 and 2 above 3
	poset.addRelation(1,3);
	poset.addRelation(2,3);
	assert(poset.hasRelation(1,3));
	assert(poset.hasRelation(2,3));

	// Set up 4 above 5 and 6
	poset.addRelation(4,5);
	poset.addRelation(4,6);
	assert(poset.hasRelation(4,5));
	assert(poset.hasRelation(4,6));

	// Now we put 3 above 4
	poset.addRelation(3,4);
	assert(poset.hasRelation(3,4));

	// Check grand-relations
	assert(poset.hasRelation(3,5));
	assert(poset.hasRelation(3,6));
	assert(poset.hasRelation(1,4));
	assert(poset.hasRelation(2,4));

	// We also need great-grand-relations!
	assert(poset.hasRelation(1,5));
	assert(poset.hasRelation(1,6));
	assert(poset.hasRelation(2,5));
	assert(poset.hasRelation(2,6));
}


void checkBasicRelations() {
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

int main() {
	checkBasicRelations();
	checkGrandRelations();
}
