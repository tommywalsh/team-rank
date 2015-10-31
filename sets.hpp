#ifndef SETS_HPP
#define SETS_HPP

#include <set>

// Helper functions to make dealing with std::set a bit easier, and easier to read
// (at the potential expense of efficiency)

namespace sets {

template<typename T>
std::set<T> intersection(const std::set<T>& s1, const std::set<T>& s2) {
  std::set<T> result;
  std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::insert_iterator<std::set<T>>(result, result.end()));
  return result;
}

template<typename T>
std::set<T> setUnion(std::set<T>& s1, std::set<T>& s2) {
  std::set<T> result;
  std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::insert_iterator<std::set<T>>(result, result.end()));
  return result;
}

template<typename T>
std::set<T> difference(const std::set<T>& s1, const std::set<T>& s2) {
  std::set<T> result;
  std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), std::insert_iterator<std::set<T>>(result, result.end()));
  return result;
}

} // end namespace sets

#endif
