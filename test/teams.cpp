#include "teams.hpp"

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

}
