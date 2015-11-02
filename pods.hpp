#ifndef PODS_HPP
#define PODS_HPP

#include "datastore.hpp"
#include "teams.hpp"
#include "games.hpp"
#include "poset.hpp"
#include <set>

// This file contains code to help organize teams into "pods". To understand
// what a pod is, we can consider different relationships between teams based
// on game results.
//
// At the start of the season, before any games are played, there are no
// relationships between any teams, and we say that any two pairs of teams are
// "not comparable"
//
// We can now introduce the concept of teams being "win-comparable". Team A is
// "win-comparable" to Team B if:
//     * Team A has won or tied against Team B, or
//     * Team B has won or tied against a team that is win-comparable to Team B
// Note that this property is not symmetric: A being win-comparable to B does
// not mean that B is comparable to A.
//
// Two teams A and B are said to be "mutually comparable" if A is win-comparable
// to B and B is win-comparable to A.
//
// A Pod is a set of teams that are all mutually comparable.
//
// The point of organizing teams in this way is that it is possible to devise a
// scheme to quantitatively rate a set of mutually compatable teams. If A is
// win-comparable to B, we can say "A has had better results than B", but we
// can have no way of saying how much better. But, if A and B are mutually
// comparable, we can come up with numerical rating systems that let us say that
// Team A's results are, say, three times better than Team B's.

typedef std::set<Reference<Team>> Pod;

class PodDB : public DataStore<Pod> {
private:
  std::map<Reference<Team>, Reference<Pod>> teamToPodMap;
public:
  Reference<Pod> getPodForTeam(Reference<Team> team);
  virtual Reference<Pod> create(const Pod& pod);
  virtual void destroy(Reference<Pod> podRef);
};

class PodOrganizer {
private:
  PodDB& db;
  Poset<Reference<Pod>>& poset;
  void mergePods(Reference<Pod> winPod, Reference<Pod> losePod);
  void processWinLoss(Reference<Team> winner, Reference<Team> loser);

public:
  PodOrganizer(PodDB& db, Poset<Reference<Pod>>& poset);
  void processGames(const GameDB& gameDB);
};

#endif
