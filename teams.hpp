#ifndef TEAMS_HPP
#define TEAMS_HPP

#include "datastore.hpp"

#include <string>
#include <map>
#include <set>

// Simple class to represent a team. For now, only includes a single name.
struct Team {
  std::string name;
  Team(std::string n) : name(n) {}
};

// Simple data store for teams. Allows lookup by name, and access by reference.
// Does not support removal of teams.
class TeamDB
{
private:
  DataStore<Team> teamRepo;
  std::map<std::string, Reference<Team>> nameToRefMap;
  Reference<Team> addNewTeam(const std::string& name);
public:
  Reference<Team> getReference(const std::string& name);
  boost::optional<Team> read(Reference<Team> team) {return teamRepo.read(team);}
  std::set<Reference<Team>> allTeams() const {return teamRepo.allItems();}
};

#endif
