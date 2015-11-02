#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <cstddef>
#include <vector>
#include <set>
#include <boost/optional.hpp>


// Very simple CRUD-based datastore
//
// This file provides a simple system for storing objects in a repository
// such that they can be referred to by lightweight references.
//
// It is most useful in the case where you have objects that:
//  * Are relatively heavyweight objects -- relatively expensive to pass around
//  * Need to be referred to from many places in the code
//  * Often to be compared for identity, but not equality
//  * Relatively infrequently have their properties accessed


// A lightweight reference class to represent an object in the datastore
template <typename T>
class Reference
{
private:
  std::size_t m_id;
public:
  Reference(std::size_t id) : m_id(id) {}
  std::size_t id() const {return m_id;}
  bool operator < (const Reference<T>& other) const {return m_id < other.m_id;}
  bool operator == (const Reference<T>& other) const {return m_id == other.m_id;}
  bool operator != (const Reference<T>& other) const {return m_id != other.m_id;}
};


// The datastore class. This is a simple in-memory implementation with no
// persistant storage.
template <typename T>
class DataStore
{
private:
  std::vector<boost::optional<T>> db;
public:

  // Create a new object in the datastore.
  virtual Reference<T> create(const T& seed);

  // Read an object from the datastore, if it exists.
  boost::optional<T> read(Reference<T> ref) const;

  // Update a stored object.
  virtual void update(Reference<T> ref, const T& newVal);

  // Remove an object from the datastore
  virtual void destroy(Reference<T> ref);

  // Get references to all items in the datastore
  std::set<Reference<T>> allItems() const;

  // Simple begin/end iterator accessors
  typename std::vector<boost::optional<T>>::const_iterator begin() const {
    return db.begin();
  }
  typename std::vector<boost::optional<T>>::const_iterator end() const {
    return db.end();
  }
};

template<typename T>
Reference<T> DataStore<T>::create(const T& seed) {
  // Always just add on to the end, never reuse an ID
  // This is theoretically space inefficient, but practically fine for us
  auto newId = db.size();
  db.push_back(boost::optional<T>(seed));
  return Reference<T>(newId);
}

template<typename T>
boost::optional<T> DataStore<T>::read(Reference<T> ref) const {
  return db[ref.id()];
}

template<typename T>
void DataStore<T>::update(Reference<T> ref, const T& newVal) {
  db[ref.id()] = newVal;
}

template<typename T>
void DataStore<T>::destroy(Reference<T> ref) {
  // When an object is destroyed, we just leave a "hole" in its place
  // that will never be reused.
  db[ref.id()] = boost::none;
}

template<typename T>
std::set<Reference<T>> DataStore<T>::allItems() const {
  std::set<Reference<T>> items;
  for (auto i = 0; i < db.size(); ++i) {
    if (db[i]) {
      items.insert(Reference<T>(i));
    }
  }
  return items;
}


#endif
