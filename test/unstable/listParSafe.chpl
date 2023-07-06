use List;

// show that the 'parSafe' argument triggers an unstable warning on all 'list' initializers:
var l1 = new list(int, parSafe=true);
var l2 = new list(l1, parSafe=false);
var l3 = new list(0..10, parSafe=true);
var l4 = new list([1, 2, 3, 4, 5], parSafe=false);

var l5 = new list(values(), parSafe=true);
iter values() {
  for i in 0..10 do yield i;
}

if l5.parSafe then writeln("parSafe");
