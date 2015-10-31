#include "games.hpp"
#include "teams.hpp"

int main() {
	TeamDB teamDB;

	auto a = teamDB.getReference("Arlington");
	auto b = teamDB.getReference("Billerica");
	auto c = teamDB.getReference("Cambridge");

	auto g1 = Game::win(a, b);
	auto hg1 = g1.getHalfGames();
	assert(hg1.size() == 2);
	assert(hg1[0].winner == a);
	assert(hg1[0].loser == b);
	assert(hg1[1].winner == a);
	assert(hg1[1].loser == b);

	auto g2 = Game::tie(b,c);
	auto hg2 = g2.getHalfGames();
	assert(hg2.size() == 2);
	assert(hg2[0].winner == hg2[1].loser);
	assert(hg2[1].winner == hg2[0].loser);
	assert(hg2[0].winner == b || hg2[1].winner == b);
	assert(hg2[0].winner == c || hg2[1].winner == c);
}
