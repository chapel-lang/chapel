private use List;

record R {
  var name: string;
  var value: int;
}

var lst = new list(R);
lst.pushBack(new R("Alice", 1));
lst.pushBack(new R("Bob", 2));
lst.pushBack(new R("Charlie", 3));
lst.pushBack(new R("Bob", 4));

// Basic find — first match index
assert(lst.find(proc(x: R): bool { return x.name == "Bob"; }) == 1);
// No match
assert(lst.find(proc(x: R): bool { return x.name == "Zara"; }) == -1);
// Find with start
assert(lst.find(proc(x: R): bool { return x.name == "Bob"; }, start=2) == 3);
// Find with start and end (no match in range)
assert(lst.find(proc(x: R): bool { return x.name == "Bob"; }, start=0, end=0) == -1);
// Find with start and end (match in range)
assert(lst.find(proc(x: R): bool { return x.name == "Bob"; }, start=0, end=1) == 1);
// Find by value condition
assert(lst.find(proc(x: R): bool { return x.value >= 3; }) == 2);

// Empty list
var empty = new list(R);
assert(empty.find(proc(x: R): bool { return true; }) == -1);

// Functor record with proc this
record NameMatcher {
  var target: string;
  proc this(x: R): bool {
    return x.name == target;
  }
}

assert(lst.find(new NameMatcher("Bob")) == 1);
assert(lst.find(new NameMatcher("Zara")) == -1);
assert(lst.find(new NameMatcher("Bob"), start=2) == 3);
