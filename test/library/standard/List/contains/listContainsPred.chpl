private use List;

record R {
  var name: string;
  var value: int;
}

var lst = new list(R);
lst.pushBack(new R("Alice", 1));
lst.pushBack(new R("Bob", 2));
lst.pushBack(new R("Charlie", 3));

// Predicate match by field
assert(lst.contains(proc(x: R): bool { return x.name == "Bob"; }));
// Predicate no match
assert(!lst.contains(proc(x: R): bool { return x.name == "Zara"; }));
// Predicate match by value condition
assert(lst.contains(proc(x: R): bool { return x.value > 2; }));
assert(!lst.contains(proc(x: R): bool { return x.value > 10; }));

// Empty list
var empty = new list(R);
assert(!empty.contains(proc(x: R): bool { return true; }));
