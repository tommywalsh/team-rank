#ifndef RESULT_HPP
#define RESULT_HPP

#include "datastore.hpp"
#include "teams.hpp"

#include <set>

// Helper struct. We model a game as a pair of "half games".
// Each half-game has a winner and a loser.
// A game that is won consists of two identical half-games
// A tie is modelled as two half-games, where each team wins one and loses one.
struct HalfGame {
  Reference<Team> winner;
  Reference<Team> loser;
  HalfGame(Reference<Team> w, Reference<Team> l) : winner(w), loser(l) {}
};

class Game {
private:
  std::vector<HalfGame> results;
  Game(HalfGame hg1, HalfGame hg2) : results{hg1, hg2} {}
public:

  // Create a game with a winner and a loser
  static Game win(Reference<Team> winner, Reference<Team> loser) {
    return Game(HalfGame(winner, loser), HalfGame(winner, loser));
  }

  // Create a tie game
  static Game tie(Reference<Team> team1, Reference<Team> team2) {
    return Game(HalfGame(team1, team2), HalfGame(team2, team1));
  }

  std::vector<HalfGame> getHalfGames() const {return results;}

};

class GameDB : public DataStore<Game> {
private:
  std::map<Reference<Team>, std::multimap<Reference<Team>, Reference<Game>>> teamToGames;
  void insertSingleGameEntry(Reference<Team> team1, Reference<Team> team2, Reference<Game> game);
  void deleteGame(std::multimap<Reference<Team>, Reference<Game>>& map, Reference<Team> team, Reference<Game> game);

public:
  virtual Reference<Game> create(Game game);
  virtual void update(Reference<Game> gameRef, const Game& newGame);
  virtual void destroy(Reference<Game> gameRef);

  // Returns all games involving the given team
  std::set<Reference<Game>> getGames(Reference<Team> team) const;

  // Returns all games involving both of the given teams
  std::set<Reference<Game>> getGames(Reference<Team> team1, Reference<Team> team2) const;
};

#endif
