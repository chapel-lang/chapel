use Set;

record R {
  var a: int;
  
  operator ==(a:R, b:R) {
    return true;
  }

  // We need to ensure that objects of this type are
  // not only `==` equivalent, but also hash to the
  // same value, based on the internal hashtable
  // implementation.
  proc hash(): uint {
    if a == 1 || a == 2 then
      return 0;
    else
      return chpl__defaultHashWrapper(a): uint;
  }
}

var s1 = new set(R);
var s2 = new set(R);

var a = new R(1);
var b = new R(2);

// These elements are `==` equivalent, but will not be
// stored in the same hash slot in the underlying table,
// so the `_findSlot` function considers them as different
// objects.
var c = new R(3);
var d = new R(4);

s1.add(a);
s2.add(b);
s1.add(c);
s2.add(d);

printSortedSet(s1 | s2);
printSortedSet(s1 + s2);
printSortedSet(s1 - s2);
printSortedSet(s1 & s2);
printSortedSet(s1 ^ s2);
writeln(s1 <= s2);
writeln(s2 <= s1);
writeln(s1 >= s2);
writeln(s2 >= s1);

proc printSortedSet(s) {
  if s.size > 0 {
    var a: [0..#s.size] int;
    var i = 0;
    for val in s {
      a[i] = val.a; i+=1;
    }
    writeln(a.sorted());
  }
}
