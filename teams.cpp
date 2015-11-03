#include "teams.hpp"

Reference<Team> TeamDB::create(const std::string& name) {
  Team team(name);
  auto teamRef = DataStore::create(team);
  nameToRefMap.insert(std::make_pair(name, teamRef));
  return teamRef;
}

void TeamDB::update(Reference<Team> teamRef, const Team& newTeam) {
  auto oldTeam = *read(teamRef);
  nameToRefMap.erase(oldTeam.name);
  nameToRefMap.insert(std::make_pair(newTeam.name, teamRef));
  DataStore::update(teamRef, newTeam);
}

void TeamDB::destroy(Reference<Team> teamRef) {
  auto team = *read(teamRef);
  nameToRefMap.erase(team.name);
  DataStore::destroy(teamRef);
}

Reference<Team> TeamDB::getReference(const std::string& name) {
  auto entry = nameToRefMap.find(name);
  if (entry == nameToRefMap.end()) {
    return create(name);
  } else {
    return entry->second;
  }
}
