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
class TeamDB : public DataStore<Team>
{
private:
  std::map<std::string, Reference<Team>> nameToRefMap;
public:
  virtual Reference<Team> create(const std::string& name);
  virtual void update(Reference<Team>, const Team& team);
  virtual void destroy(Reference<Team> teamRef);
  Reference<Team> getReference(const std::string& name);
};

#endif
