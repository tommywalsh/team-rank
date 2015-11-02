#include "games.hpp"
#include "teams.hpp"

#include "../teams.cpp"

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

	auto g3 = Game::win(c,a);

	GameDB gameDB;
	auto gr1 = gameDB.addGame(g1);
	auto gr2 = gameDB.addGame(g2);
	auto gr3 = gameDB.addGame(g3);

	auto aGames = gameDB.getGames(a);
	assert(aGames.size() == 2);
	assert(aGames.count(gr1) == 1);
	assert(aGames.count(gr3) == 1);

	auto bGames = gameDB.getGames(b);
	assert(bGames.size() == 2);
	assert(bGames.count(gr1) == 1);
	assert(bGames.count(gr2) == 1);

	auto cGames = gameDB.getGames(c);
	assert(cGames.size() == 2);
	assert(cGames.count(gr2) == 1);
	assert(cGames.count(gr3) == 1);

	auto acGames = gameDB.getGames(a,c);
	assert(acGames.size() == 1);
	assert(acGames.count(gr3) == 1);

	auto g4 = Game::win(c,a);
	auto gr4 = gameDB.addGame(g4);
 	acGames = gameDB.getGames(a,c);
	assert(acGames.size() == 2);
	assert(acGames.count(gr3) == 1);
	assert(acGames.count(gr4) == 1);

}
