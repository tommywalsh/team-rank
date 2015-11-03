#include "games.hpp"

std::pair<Reference<Team>,Reference<Team>> getTeams(const Game& game) {
  auto hg = *(game.getHalfGames().begin());
  return {hg.winner, hg.loser};
}

// Each game is stored twice (to ease lookups by either team). This method stores one.
void GameDB::insertSingleGameEntry(Reference<Team> team1, Reference<Team> team2, Reference<Game> game) {
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


void GameDB::deleteGame(std::multimap<Reference<Team>, Reference<Game>>& map, Reference<Team> team, Reference<Game> game) {
  auto range = map.equal_range(team);
  for (auto i = range.first; i != range.second; ++i) {
    if (i->second == game) {
      map.erase(i);
      return;
    }
  }
}

Reference<Game> GameDB::create(Game game) {
  auto gameRef = DataStore::create(game);
  auto teams = getTeams(game);
  insertSingleGameEntry(teams.first, teams.second, gameRef);
  insertSingleGameEntry(teams.second, teams.first, gameRef);
  return gameRef;
}

void GameDB::destroy(Reference<Game> gameRef) {
  auto game = *read(gameRef);
  auto teams = getTeams(game);
  deleteGame(teamToGames[teams.first], teams.second, gameRef);
  deleteGame(teamToGames[teams.second], teams.first, gameRef);
  DataStore::destroy(gameRef);
}

void GameDB::update(Reference<Game> gameRef, const Game& newGame) {
  auto oldGame = *read(gameRef);
  DataStore::update(gameRef, newGame);

  // Remove our indexing of the old game
  auto teams = getTeams(oldGame);
  deleteGame(teamToGames[teams.first], teams.second, gameRef);
  deleteGame(teamToGames[teams.second], teams.first, gameRef);

  // Add indexing of the new game
  teams = getTeams(newGame);
  insertSingleGameEntry(teams.first, teams.second, gameRef);
  insertSingleGameEntry(teams.second, teams.first, gameRef);
}

// Returns all games involving the given team
std::set<Reference<Game>> GameDB::getGames(Reference<Team> team) const {
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
std::set<Reference<Game>> GameDB::getGames(Reference<Team> team1, Reference<Team> team2) const {
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
