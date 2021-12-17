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
    return 0;
  }
}

var s1 = new set(R);
var s2 = new set(R);

var a = new R("LHS");
var b = new R("RHS");

s1.add(a);
s2.add(b);

writeln(s1 | s2);
writeln(s1 + s2);
writeln(s1 - s2);
writeln(s1 & s2);
writeln(s1 ^ s2);
writeln(s1 <= s2);
writeln(s2 <= s1);
writeln(s1 >= s2);
writeln(s2 >= s1);
