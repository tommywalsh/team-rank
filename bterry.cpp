/*

 ====== Explanation of Bradley-Terry algorithm ======

Calculation of the Braley-Terry score for a team i is done iteratively using this formula

Si' := Wi * SOSi             "new score of team i"

Wi - Total number of wins by team i

SOSi := 1 / SUM(j: Cij)      "strength of schedule of team i"

Cij := Gij / (Si + Sj)       "Comparator between teams i and j"

Gij - Total number of games between teams i and j

Si - Prior score of team i


In addition, we want our score normalized such than a theoretic "average team" has a score of 100.

We begin by arbitrarily assigning a score of 100 to everyone, then iterating until the scores converge.

To reduce special cases with ties, we actually work with half-games and half-wins in all these calculations.

*/

#include "bterry.hpp"

namespace {

// Syntax helper for retrieving from a const map, when you know the item is there
template <typename Key, typename Val>
Val get(const std::map<Key, Val>& map, const Key& key) {
  return map.find(key)->second;
}

typedef Reference<Team> TeamRef;
typedef std::map<TeamRef, double> ScoreList;
typedef std::map<TeamRef, std::map<TeamRef, int>> WinMatrix;

// Lookup matrix
// matrix[i][j] is the number of times i has beaten j
WinMatrix getWinMatrix(const Pod& pod, const GameDB& gameDB) {
  WinMatrix wins;
  for (auto t1 : pod) {
    for (auto t2 : pod) {
      for (auto gameRef : gameDB.getGames(t1,t2)) {
        auto game = *(gameDB.read(gameRef));
        // Really, we count half-game wins
        for (auto halfGame : game.getHalfGames()) {
          if (halfGame.winner == t1) {
            // If team1 has won, then record it.
            // If team2 is the winner, then skip it.
            wins[t1][t2]++;
          }
        }
      }
    }
  }
  return wins;
}

int getWins(const WinMatrix& wins, TeamRef a, TeamRef b) {
  int total = 0;
  auto teamWins = wins.find(a);
  if (teamWins != wins.end()) {
    auto vsWins = teamWins->second.find(b);
    if (vsWins != teamWins->second.end()) {
      total += vsWins->second;
    }
  }
  return total;
}

int getGameCount(const WinMatrix& wins, TeamRef a, TeamRef b) {
  return getWins(wins, a, b) + getWins(wins, b, a);
}

double getComparator(const WinMatrix& wins, const ScoreList& scores, TeamRef a, TeamRef b) {
  return getGameCount(wins, a, b) / (get(scores, a) + get(scores, b));
}

double getScheduleStrength(const Pod& pod, const WinMatrix& wins, const ScoreList& scores, TeamRef team) {
  double compSum = 0;
  for (auto other : pod) {
    compSum += getComparator(wins, scores, team, other);
  }

  return 1.0 / compSum;
}

int getWinTotal(const Pod& pod, const WinMatrix& wins, TeamRef team) {
  int total = 0;
  auto teamWins = wins.find(team);
  if (teamWins != wins.end()) {
    for (auto other : pod) {
      auto vsWins = teamWins->second.find(other);
      if (vsWins != teamWins->second.end()) {
        total += vsWins->second;
      }
    }
  }
  return total;
}

double getRawScore(const Pod& pod, const WinMatrix& wins, const ScoreList& oldScores, TeamRef team) {
  return getWinTotal(pod, wins, team) * getScheduleStrength(pod, wins, oldScores, team);
}

ScoreList normalizeScores(const ScoreList& oldScores) {

  // We'll need to know the sum of scores and the number of scores.
  int count = 0;
  double sum = 0;
  for (auto entry: oldScores) {
    count++;
    sum += entry.second;
  }

  // We want the average team to have a score of 100, so that means we want
  // the new scores to sum to 100*count.
  double target = 100.0 * count;
  double multiplier = target / sum;

  ScoreList newScores;
  for (auto entry: oldScores) {
    newScores[entry.first] = entry.second * multiplier;
  }
  return newScores;
}

ScoreList getInitialScoreList(const Pod& pod) {
  ScoreList scores;
  for (auto team : pod) {
    scores[team] = 100;
  }
  return scores;
}

} // end anon namespace

ScoreList BradleyTerry::calculateScores(const Pod& pod, const GameDB& db) {
  ScoreList scores = getInitialScoreList(pod);

  // Only do work if pod has more than one team
  if (pod.size() < 2) {
    return scores;
  }

  WinMatrix winMatrix = getWinMatrix(pod, db);

  // Really, we should be looping until our precision is high enough,
  // but looping a fixed number of times will do for now.
  for (auto i = 0; i < 90; ++i) {
    ScoreList newScores;
    for (auto team : pod) {
      newScores[team] = getRawScore(pod, winMatrix, scores, team);
    }
    scores = normalizeScores(newScores);
  }
}
