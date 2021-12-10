use Set;

record R {
  var a: string;
  
  operator ==(a:R, b:R) {
    return true;
  }

  // We need to ensure that objects of this type are
  // not only `==` equivalent, but also hash to the
  // same value, based on the internal hashtable
  // implementation.
  proc hash(): uint {
    if a == "LHS" || a == "RHS" then
      return 0;
    else
      return chpl__defaultHashWrapper(a): uint;
  }
}

var s1 = new set(R);
var s2 = new set(R);

var a = new R("LHS");
var b = new R("RHS");
var c = new R("LHS other");
var d = new R("RHS other");

s1.add(a);
s2.add(b);
s1.add(c);
s2.add(d);

writeln(s1 | s2);
writeln(s1 + s2);
writeln(s1 - s2);
writeln(s1 & s2);
writeln(s1 ^ s2);
writeln(s1 <= s2);
writeln(s2 <= s1);
writeln(s1 >= s2);
writeln(s2 >= s1);
