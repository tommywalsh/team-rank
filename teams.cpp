#include "teams.hpp"

Reference<Team> TeamDB::addNewTeam(const std::string& name) {
  Team team(name);
  auto teamRef = teamRepo.create(team);
  nameToRefMap.insert(std::make_pair(name, teamRef));
  return teamRef;
}

Reference<Team> TeamDB::getReference(const std::string& name) {
  auto entry = nameToRefMap.find(name);
  if (entry == nameToRefMap.end()) {
    return addNewTeam(name);
  } else {
    return entry->second;
  }
}
