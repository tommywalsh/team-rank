#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <cstddef>
#include <vector>
#include <set>
#include <optional>


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
  std::vector<std::optional<T>> db;
public:

  // Create a new object in the datastore.
  virtual Reference<T> create(const T& seed);

  // Read an object from the datastore, if it exists.
  std::optional<T> read(Reference<T> ref) const;

  // Tests a reference for validity
  bool isValid(Reference<T> ref) const;

  // Update a stored object.
  virtual void update(Reference<T> ref, const T& newVal);

  // Remove an object from the datastore
  virtual void destroy(Reference<T> ref);

  // Get references to all items in the datastore
  std::set<Reference<T>> allItems() const;

  // Iterator that visits all valid objects in datastore (skipping over the holes)
  class iterator : public std::iterator <std::input_iterator_tag, T> {
    const DataStore* ds;
    std::size_t index;

    Reference<T> ref() const {
      return Reference<T>(index);
    }

    void advanceIfNecessary() {
      if (ds) {
        while (index < ds->db.size()) {
          auto r = ref();
          if (ds->isValid(r)) return;
          ++index;
        }
        // If we get here, we're out of objects
        ds = nullptr;
      }
    }
  public:
    iterator() : ds(nullptr) {}
    iterator(const DataStore* dataStore) : ds(dataStore), index(0) {
      advanceIfNecessary();
    }
    iterator(const iterator& other) : ds(other.ds), index(other.index) {}

    Reference<T> operator * () const {
      return ref();
    }

    iterator& operator++ () {
      ++index;
      advanceIfNecessary();
      return *this;
    }

    iterator operator++ (int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator == (const iterator& other) {

      // Different datastores -> not the same
      if (ds != other.ds) return false;

      // Neither have a datastore -> the same
      if (!ds) return true;

      // Otherwise, compare indices
      return index == other.index;
    }

    bool operator != (const iterator& other) {
      return !(*this == other);
    }
  };

  iterator begin() const {
    return iterator(this);
  }

  iterator end() const {
    return iterator();
  }

};

template<typename T>
Reference<T> DataStore<T>::create(const T& seed) {
  // Always just add on to the end, never reuse an ID
  // This is theoretically space inefficient, but practically fine for us
  auto newId = db.size();
  db.push_back(std::optional<T>(seed));
  return Reference<T>(newId);
}

template<typename T>
std::optional<T> DataStore<T>::read(Reference<T> ref) const {
  return db[ref.id()];
}

template<typename T>
bool DataStore<T>::isValid(Reference<T> ref) const {
  auto id = ref.id();
  return (id < db.size()) && (db[id]);
}

template<typename T>
void DataStore<T>::update(Reference<T> ref, const T& newVal) {
  db[ref.id()] = newVal;
}

template<typename T>
void DataStore<T>::destroy(Reference<T> ref) {
  // When an object is destroyed, we just leave a "hole" in its place
  // that will never be reused.
  db[ref.id()] = std::nullopt;
}

template<typename T>
std::set<Reference<T>> DataStore<T>::allItems() const {
  return std::set<Reference<T>>(begin(), end());
}

#endif
