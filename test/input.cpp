#include "input.hpp"

#include "teams.hpp"
#include "games.hpp"

#include <sstream>

int main() {
  TeamDB teamDB;
  GameDB gameDB;

  InputProcessor processor(teamDB, gameDB);
  std::string input = "Arlington 3, Boston 7\n";
  input.append("Cambridge 2, Boston 2\n");
  input.append("Cambridge 12, Danvers 0\n");
  input.append("Danvers 2, Arlington 2\n");
  input.append("Arlington 1, Cambridge 4\n");

  std::istringstream stream(input);
  processor.process(stream);

  assert(teamDB.allTeams().size() == 4);
  assert(gameDB.allItems().size() == 5);
}
