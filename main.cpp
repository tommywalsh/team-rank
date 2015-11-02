
#include "input.hpp"
#include "teams.hpp"
#include "games.hpp"
#include "pods.hpp"

int main() {
  TeamDB teamDB;
  GameDB gameDB;
  PodDB podDB;
  Poset<Reference<Pod>> podPoset;

  // Grab results from stdin and store teams and game results
  InputProcessor inputProcessor(teamDB, gameDB);
  inputProcessor.process(std::cin);

  // Organize teams into pods based on game results
  PodOrganizer podOrganizer(podDB, podPoset);
  podOrganizer.processGames(gameDB);

  std::cout << "We have " << podDB.allItems().size()
    << " pods, with " << podPoset.getTopLevel().size()
    << " top level pods.\n";

}
