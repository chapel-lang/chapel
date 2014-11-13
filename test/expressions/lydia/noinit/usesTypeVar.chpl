proc _defaultOf(type t) where (isBoolType(t)) {
  writeln("I default initialized!");
  return false:t;
}


type a = bool;
var foo: a = noinit; // Should not print out the message
foo = true;
writeln(foo);
var bar: a;          // Should print out the message
writeln(bar);
