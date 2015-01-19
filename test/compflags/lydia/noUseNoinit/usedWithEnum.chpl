enum enumt { foo=0, bar=1, baz=2};

inline proc _defaultOf(type t) where t: enumt {
  writeln("I default initialized!");
  return enumt.foo;
}

var blah: enumt = noinit; // Should not print out the message
blah = enumt.bar;
writeln(blah);
var blah2: enumt;         // Should print out the message
writeln(blah2);
