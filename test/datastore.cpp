#include "datastore.hpp"

#include <cassert>

void assertSize(DataStore<int>& ds, std::size_t expectedSize) {
  assert (ds.allItems().size() == expectedSize);
}

int main()
{
  DataStore<int> store;

  assertSize(store, 0);
  auto a = store.create(5);
  assertSize(store, 1);
  auto b = store.create(10);
  assertSize(store, 2);
  assert(a != b);
  assert(store.isValid(b));

  auto c = store.read(a);
  assert(c);
  assert(*c == 5);

  assertSize(store, 2);
  store.update(a, 20);
  assertSize(store, 2);
  auto d = store.read(a);
  assertSize(store, 2);
  assert(d);
  assert(*d == 20);

  assert(store.read(b));
  store.destroy(b);
  assert(!store.isValid(b));
  assert(!store.read(b));
  assertSize(store, 1);

  store.create(0);
  assertSize(store, 2);

}
