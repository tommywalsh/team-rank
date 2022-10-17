#include "pods.hpp"

#include "sets.hpp"



Reference<Pod> PodDB::getPodForTeam(Reference<Team> team) {
  auto it = teamToPodMap.find(team);
  if (it != teamToPodMap.end()) {
    return it->second;
  } else {
    return create({team});
  }
}

Reference<Pod> PodDB::create(const Pod& pod) {
  auto podRef = DataStore::create(pod);
  for (auto team : pod) {
    teamToPodMap.insert(std::make_pair(team, podRef));
  }
  return podRef;
}

void PodDB::destroy(Reference<Pod> podRef) {
  auto teamsInPod = *read(podRef);
  for (auto team : teamsInPod) {
    teamToPodMap.erase(team);
  }
  DataStore::destroy(podRef);
}


void PodOrganizer::mergePods(Reference<Pod> winPod, Reference<Pod> losePod) {
  auto aboveWinner = poset.getAbove(winPod);
  auto belowWinner = poset.getBelow(winPod);
  auto aboveLoser = poset.getAbove(losePod);
  auto belowLoser = poset.getBelow(losePod);

  aboveWinner.erase(losePod);
  belowLoser.erase(winPod);

  // The given pods will be merged into a single new pod.
  // Other pods must fall into one of these existing sets:
  // Related to neither winner nor loser -> no change
  // Related to only one of winner or loser -> same relation to new pod
  // Related to both:
  //   above both -> above the new pod
  //   below both -> below the new pod
  //   between -> merged into the new pod
  auto allAboves = sets::setUnion(aboveWinner, aboveLoser);
  auto allBelows = sets::setUnion(belowWinner, belowLoser);
  auto inBetween = sets::intersection(allAboves, allBelows);

  auto toBeAbove = sets::difference(allAboves, inBetween);
  auto toBeBelow = sets::difference(allBelows, inBetween);

  auto toBeMerged = inBetween;
  toBeMerged.insert(winPod);
  toBeMerged.insert(losePod);

  Pod newPod;
  for (auto podRef : toBeMerged) {

    // Remember the teams in this pod
    auto pod = *(db.read(podRef));
    newPod.insert(pod.begin(), pod.end());

    // Take this pod out of our poset
    poset.removeObject(podRef);

    // And, remove it from the database
    db.destroy(podRef);
  }

  // Add the newly-merged pod to the database
  auto podRef = db.create(newPod);

  // And set up the new pod's relations
  for (auto above : toBeAbove) {
    poset.addRelation(above, podRef);
  }
  for (auto below : toBeBelow) {
    poset.addRelation(podRef, below);
  }
}

void PodOrganizer::processWinLoss(Reference<Team> winner, Reference<Team> loser) {
  auto winPod = db.getPodForTeam(winner);
  auto losePod = db.getPodForTeam(loser);

  auto aboveWinner = poset.getAbove(winPod);
  if (aboveWinner.count(losePod) == 0) {
    poset.addRelation(winPod, losePod);
  } else {
    mergePods(winPod, losePod);
  }
}

PodOrganizer::PodOrganizer(PodDB& pdb, Poset<Reference<Pod>>& pst) :
  db(pdb),
  poset(pst)
  {}


void PodOrganizer::processGames(const GameDB& gameDB) {
  for (auto gameRef : gameDB.allItems()) {
    auto game = gameDB.read(gameRef);
    for (auto halfGame : game->getHalfGames()) {
      processWinLoss(halfGame.winner, halfGame.loser);
    }
  }
}
