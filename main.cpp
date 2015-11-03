
#include "input.hpp"
#include "teams.hpp"
#include "games.hpp"
#include "pods.hpp"
#include "bterry.hpp"

#include <iomanip>

void prettyPrint(std::ostream& os, double val) {
  os << std::setw(5);
  if (val >= 100) {
    os << std::setprecision(0) <<std::fixed << val;
  } else if (val >= 10) {
    os << std::setprecision(1) << std::fixed << val;
  } else if (val >= 1) {
    os << std::setprecision(2) << std::fixed << val;
  } else {
    os << std::setprecision(3) << std::fixed << val;
  }
}

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

  for (auto podRef : podDB.allItems()) {

    std::cout << "\n\nPod #" << podRef.id() << "\n";
    auto above = podPoset.getAbove(podRef);
    if (above.size() > 0) {
      std::cout << "(Below these pods:";
      for (auto other : above) {
        std::cout << " " << other.id();
      }
      std::cout << ")\n";
    }

    auto pod = *(podDB.read(podRef));
    auto scores = BradleyTerry::calculateScores(pod, gameDB);

    // Sort by scores
    std::vector<Reference<Team>> sortedTeams(pod.begin(), pod.end());
    auto sorter = [scores](Reference<Team> a, Reference<Team> b){
      return scores.find(a)->second > scores.find(b)->second;
    };

    std::sort(sortedTeams.begin(), sortedTeams.end(), sorter);

    for (auto team : sortedTeams) {
      std::cout << "     ";
      prettyPrint(std::cout, scores[team]);
      std::cout << " " << teamDB.read(team)->name << "\n";
    }
  }

}
