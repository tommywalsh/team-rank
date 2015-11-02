#ifndef INPUT_HPP
#define INPUT_HPP

#include "teams.hpp"
#include "games.hpp"

class InputProcessor {
  TeamDB& teamDB;
  GameDB& gameDB;
public:
  InputProcessor(TeamDB& teamDB, GameDB& gameDB);
  void process(std::istream& is);
};

#endif
