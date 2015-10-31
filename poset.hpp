#ifndef POSET_HPP
#define POSET_HPP

#include <set>
#include <map>

// This class reprensents a simple partially-ordered set.
//
// This is a set of relations from A to B that is:
//    * Antisymmetric (If A is related to B, then B is not related to A)
//    * Antireflexive (A is never related to A)
//    * Transitive (If A is related to B and B is related to C, then A is related to C)
// Additionally, there is no requirement that all pairs have a relation defined.
//
// If A is related to B, we say A is "above" B and that B is "below" A.
//
// This implementation favors quickness over memory efficiency, so it is advised
// to only use this class with small objects.
template <typename T>
class Poset {
public:
  std::map<T,std::set<T>> ancestors;
  std::map<T,std::set<T>> descendants;
public:

  // Add a new relation to the poset. This will also deduce any additional transitive relations
  void addRelation(T above, T below);

  // Any relations involving this object are deleted. This will *not* delete any other relations,
  // even if those relations were deduced using this object.
  void removeObject(T oldObj);

  // Tells if the specifiied relation is in the poset.
  bool hasRelation(T maybeAbove, T maybeBelow) const;

  // Get a list of all object above or below the given object.
  std::set<T> getAbove(T) const;
  std::set<T> getBelow(T) const;

  // Returns all the objects that have no other objects above them.
  std::set<T> getTopLevel() const;
};

template <typename T>
std::set<T> Poset<T>::getTopLevel() const {
  std::set<T> topLevel;
  for (auto entry : ancestors) {
    if (entry.second.empty()) {
      topLevel.insert(entry.first);
    }
  }
  return topLevel;
}

template <typename T>
std::set<T> Poset<T>::getAbove(T below) const {
  return ancestors.at(below);
}

template <typename T>
std::set<T> Poset<T>::getBelow(T above) const {
  return descendants.at(above);
}


template <typename T>
bool Poset<T>::hasRelation(T maybeAbove, T maybeBelow) const {
  auto aboveBelow = ancestors.find(maybeBelow);
  if (aboveBelow == ancestors.end()) {
    return false;
  }
  return (aboveBelow->second.count(maybeAbove) != 0);
}

template <typename T>
void Poset<T>::addRelation(T above, T below) {

  auto grandAncestors = ancestors[above];
  for (auto ancestor : grandAncestors) {
    descendants[ancestor].insert(below);
  }
  ancestors[below].insert(grandAncestors.begin(), grandAncestors.end());
  ancestors[below].insert(above);

  auto grandDescendants = descendants[below];
  for (auto descendant : grandDescendants) {
    ancestors[descendant].insert(above);
  }
  descendants[above].insert(grandDescendants.begin(), grandDescendants.end());
  descendants[above].insert(below);

}

template <typename T>
void Poset<T>::removeObject(T oldObj) {
  ancestors.erase(oldObj);
  descendants.erase(oldObj);

  for (auto parents : ancestors) {
    ancestors[parents.first].erase(oldObj);
  }

  for (auto children : descendants) {
    descendants[children.first].erase(oldObj);
  }
}

#endif
