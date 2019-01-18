proc _defaultOf(type t:bool) {
  writeln("I default initialized!");
  return false:t;
}

var foo: bool;          // Should print out the message
writeln("Should follow the first default initialization message");
var bar: bool = noinit;
// Should still print out the message, because of compopts
