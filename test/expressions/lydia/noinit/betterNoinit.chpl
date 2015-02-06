proc _defaultOf(type t) where (isBoolType(t)) {
  writeln("I default initialized!");
  return false:t;
}

var foo: bool;          // Should print out the message
writeln("Should follow the default initialization message");
var bar: bool = noinit; // Should not print out the message.
