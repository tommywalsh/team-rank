#include "teams.hpp"

#include <cassert>

int main() {
	TeamDB teamDB;

	auto a = teamDB.getReference("Arlington");
	auto at = teamDB.read(a);
	assert(at);
	assert(at->name == "Arlington");

	auto b = teamDB.getReference("Billerica");
	assert (b != a);
	auto b2 = teamDB.getReference("Billerica");
	assert(b == b2);
	auto bt = teamDB.read(b);
	auto b2t = teamDB.read(b2);
	assert(bt);
	assert(b2t);
	assert(bt->name == b2t->name);

	assert(teamDB.allItems().size() == 2);
	teamDB.destroy(b);
	assert(teamDB.allItems().size() == 1);
	teamDB.update(a, Team("Andover"));
	assert(teamDB.allItems().size() == 1);
	assert(teamDB.read(a)->name == "Andover");
	auto a2 = teamDB.getReference("Andover");
	auto a3 = teamDB.getReference("Arlington");
	assert(a2 != a3);
	assert(teamDB.allItems().size() == 2);
}
