#include "pods.hpp"

#include "teams.hpp"
#include "games.hpp"

#include <cassert>

void testPodDB() {
  TeamDB teamDB;
  auto a = teamDB.getReference("Arlington");
  auto b = teamDB.getReference("Boston");
  auto c = teamDB.getReference("Cambridge");
  auto d = teamDB.getReference("Danvers");
  auto e = teamDB.getReference("Everett");

  PodDB podDB;
  auto abc = podDB.create({a,b,c});
  assert(podDB.getPodForTeam(a) == abc);
  assert(podDB.getPodForTeam(b) == abc);
  assert(podDB.getPodForTeam(c) == abc);
  auto dPod1 = podDB.getPodForTeam(d);
  assert(dPod1 != abc);
  podDB.destroy(dPod1);
  auto dPod2 = podDB.getPodForTeam(d);
  assert(dPod2 != abc);
  assert(dPod2 != dPod1);
}

void testPodOrganizer() {
  TeamDB teamDB;
  auto a = teamDB.getReference("Arlington");
  auto b = teamDB.getReference("Boston");
  auto c = teamDB.getReference("Cambridge");
  auto d = teamDB.getReference("Danvers");
  auto e = teamDB.getReference("Everett");

  GameDB gameDB;
  gameDB.create(Game::win(a,b));
  gameDB.create(Game::tie(c,d));
  gameDB.create(Game::win(e,a));
  gameDB.create(Game::win(e,c));

  PodDB podDB;
  Poset<Reference<Pod>> poset;
  PodOrganizer po(podDB, poset);
  po.processGames(gameDB);

  //      {e}
  //      / \
  // {c,d}  {a}
  //         |
  //        {b}
  assert(podDB.allItems().size() == 4);
  auto aPod = podDB.getPodForTeam(a);
  auto bPod = podDB.getPodForTeam(b);
  auto cPod = podDB.getPodForTeam(c);
  auto dPod = podDB.getPodForTeam(d);
  auto ePod = podDB.getPodForTeam(e);
  assert(cPod == dPod);
  assert(poset.getTopLevel().size() == 1);
  assert(poset.hasRelation(ePod, cPod));
  assert(poset.hasRelation(ePod, aPod));
  assert(poset.hasRelation(ePod, bPod));
  assert(poset.hasRelation(aPod, bPod));
  assert(!poset.hasRelation(cPod, ePod));
  assert(!poset.hasRelation(cPod, aPod));
  assert(!poset.hasRelation(cPod, bPod));
  assert(!poset.hasRelation(aPod, ePod));
  assert(!poset.hasRelation(aPod, cPod));
  assert(!poset.hasRelation(bPod, ePod));
  assert(!poset.hasRelation(bPod, cPod));
  assert(!poset.hasRelation(bPod, aPod));


  // Try again, but now let's have b beat e.
  // This should cause the {e} {a} and {b} pods to be merged.
  //     {a,b,e}
  //        |
  //      {c,d}
  gameDB.create(Game::win(b,e));
  PodDB podDB2;
  Poset<Reference<Pod>> poset2;
  PodOrganizer po2(podDB2, poset2);
  po2.processGames(gameDB);

  assert(podDB2.allItems().size() == 2);
  aPod = podDB2.getPodForTeam(a);
  bPod = podDB2.getPodForTeam(b);
  cPod = podDB2.getPodForTeam(c);
  dPod = podDB2.getPodForTeam(d);
  ePod = podDB2.getPodForTeam(e);
  assert(cPod == dPod);
  assert(aPod == bPod);
  assert(aPod == ePod);
  assert(poset2.getTopLevel().size() == 1);
  assert(poset2.hasRelation(aPod, cPod));
}

int main() {
  testPodDB();
  testPodOrganizer();
}
