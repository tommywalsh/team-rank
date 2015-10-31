#ifndef RESULT_HPP
#define RESULT_HPP

#include "datastore.hpp"
#include "teams.hpp"

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
public:
  std::vector<HalfGame> results;
private:
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

// Simple datastore for games.
// Only supports adding games. Retrieval of games will come later when we need it.
class GameDB {
private:
  DataStore<Game> datastore;
  std::map<Reference<Team>, std::vector<Reference<Game>>> teamToGames;
public:
  void addGame(Game game) {
    auto gameRef = datastore.create(game);
    teamToGames[game.results[0].winner].push_back(gameRef);
    teamToGames[game.results[0].loser].push_back(gameRef);
  }
};

#endif
