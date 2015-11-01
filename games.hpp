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

// Simple datastore for games.
// Only supports adding games. Retrieval of games will come later when we need it.
class GameDB {
private:
  DataStore<Game> datastore;
  std::map<Reference<Team>, std::multimap<Reference<Team>, Reference<Game>>> teamToGames;

  // Each game is stored twice (to ease lookups by either team). This method stores one.
  void insertSingleGameEntry(Reference<Team> team1, Reference<Team> team2, Reference<Game> game) {
    auto teamMap = teamToGames.find(team1);
    if (teamMap == teamToGames.end()) {
      // First game involving team1. Make everything from scratch
      std::multimap<Reference<Team>, Reference<Game>> subMap;
      subMap.insert(std::make_pair(team2, game));
      teamToGames.insert(std::make_pair(team1, subMap));
    } else {
      // We've already seen team1
      teamMap->second.insert(std::make_pair(team2, game));
    }
  }

public:
  Reference<Game> addGame(Game game) {
    auto gameRef = datastore.create(game);
    auto hg = *(game.getHalfGames().begin());
    auto team1 = hg.winner;
    auto team2 = hg.loser;

    insertSingleGameEntry(team1, team2, gameRef);
    insertSingleGameEntry(team2, team1, gameRef);

    return gameRef;
  }

  // Returns all games involving the given team
  std::set<Reference<Game>> getGames(Reference<Team> team) {
    std::set<Reference<Game>> games;
    auto subMap = teamToGames.find(team);
    if (subMap != teamToGames.end()) {
      for (auto entry : subMap->second) {
        games.insert(entry.second);
      }
    }
    return games;
  }

  // Returns all games involving both of the given teams
  std::set<Reference<Game>> getGames(Reference<Team> team1, Reference<Team> team2) {
    std::set<Reference<Game>> games;
    auto entry = teamToGames.find(team1);
    if (entry != teamToGames.end()) {
      auto subMap = entry->second;
      auto range = subMap.equal_range(team2);
      for (auto i = range.first; i != range.second; ++i) {
        games.insert(i->second);
      }
    }
    return games;
  }
};

#endif
