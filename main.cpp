
#include "input.hpp"
#include "teams.hpp"
#include "games.hpp"
#include "pods.hpp"
#include "bterry.hpp"

#include <iomanip>
#include <iostream>
#include <algorithm>

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

void printPodSimply(const TeamDB& teamDB, const std::map<Reference<Team>, double>& scoreReport, const std::string& prefix = "") {

  // Sort by scores
  std::vector<Reference<Team>> sortedTeams;
  for (auto entry: scoreReport) {
    sortedTeams.push_back(entry.first);
  }
  auto sorter = [&scoreReport](Reference<Team> a, Reference<Team> b){
    return scoreReport.find(a)->second > scoreReport.find(b)->second;
  };
  std::sort(sortedTeams.begin(), sortedTeams.end(), sorter);

  for (auto team : sortedTeams) {
    std::cout << prefix;
    prettyPrint(std::cout, scoreReport.find(team)->second);
    std::cout << " " << teamDB.read(team)->name << "\n";
  }
}

void printPodReport(const TeamDB& teamDB, std::map<Reference<Pod>, std::map<Reference<Team>, double>>& podScoreMap, const Poset<Reference<Pod>>& poset) {
  if (podScoreMap.size() == 1) {
    printPodSimply(teamDB, podScoreMap.begin()->second);
  } else {
    for (auto entry: podScoreMap) {
      auto podRef = entry.first;
      std::cout << "\n\nPod #" << podRef.id() << "\n";
      auto above = poset.getAbove(podRef);
      if (above.size() > 0) {
        std::cout << "(which is below these pods:";
        for (auto other : above) {
          std::cout << " " << other.id();
        }
        std::cout << ")\n";
      }
      printPodSimply(teamDB, entry.second, "     ");
    }
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

  // Calculate ratings within each pod
  std::map<Reference<Pod>, std::map<Reference<Team>, double>> podScoreMap;
  for (auto podRef : podDB) {
    auto pod = *(podDB.read(podRef));
    podScoreMap[podRef] = BradleyTerry::calculateScores(pod, gameDB);
  }

  // Print out report for the pods
  printPodReport(teamDB, podScoreMap, podPoset);
}
