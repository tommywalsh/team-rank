#ifndef BTERRY_HPP
#define BTERRY_HPP

#include "games.hpp"
#include "pods.hpp"

// This provides a Bradley-Terry score for each team in the pod.
//
// Bradley-Terry scores have these properties:
//  * Based only on who won, who lost, and who tied
//      - No consideration to score difference, "momentum", "luck", home field, etc.
//  * Ratios between teams are what's important
//      - If A's score is 2x B's score, you'd expect A to win 2-of-3 against B
//  * Strength of schedule matters
//      - Beating a team with a higher score is better than beating a team with a lower score.
//      - Losing to a team with a lower score is worse than losing to a team with a higher score.
//  * Winning is always good and losing is always bad.
//      - A team's (relative) score always goes up after a win, even against a terrible team.
//      - A team's (relative) score always goes down after a loss, even against a great team.
//
// Bradley-Terry scores can only be calculated for a "pod" of teams. In a pod, each team must
//   be connected by a string of wins or ties to each other team.
namespace BradleyTerry {
  std::map<Reference<Team>, double> calculateScores(const Pod& pod, const GameDB& gameDB);
}

#endif
