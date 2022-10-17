#include "input.hpp"

#include <string>
#include <iostream>
#include <cassert>

namespace {

  void addGame(TeamDB& teamDB, GameDB& gameDB,
    const std::string& name1, int score1,
    const std::string& name2, int score2) {
      auto team1 = teamDB.getReference(name1);
      auto team2 = teamDB.getReference(name2);

      if (score1 > score2) {
        gameDB.create(Game::win(team1, team2));
      } else if (score1 < score2) {
        gameDB.create(Game::win(team2, team1));
      } else {
        gameDB.create(Game::tie(team1, team2));
      }
  }

  void processLine(TeamDB& teamDB, GameDB& gameDB, const std::string& line) {
    auto comma = line.find(',');
    assert(comma != std::string::npos);
    std::string first(line, 0, comma);
    std::string second(line, comma+2);

    auto space = first.rfind(' ');
    std::string name1(first, 0, space);
    auto score1 = stoi(std::string(first, space+1));

    space = second.rfind(' ');
    std::string name2(second, 0, space);
    auto score2 = stoi(std::string(second, space+1));

    addGame(teamDB, gameDB, name1, score1, name2, score2);
  }
}

InputProcessor::InputProcessor(TeamDB& tdb, GameDB& gdb) :
  teamDB(tdb),
  gameDB(gdb)
  {}

void InputProcessor::process(std::istream& is) {
  std::string line;
  while (std::getline(is, line)) {
    processLine(teamDB, gameDB, line);
  }
}
