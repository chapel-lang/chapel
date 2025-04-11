proc returnsBool() {
  writeln("evaluated");
  return true;
}

var x : bool;

writeln("shouldn't short-circuit for &&= with LHS true");
x = true;
x &&= returnsBool();
writeln();

writeln("should short-circuit for &&= with LHS false");
x = false;
x &&= returnsBool();
writeln();

writeln("should short-circuit for ||= with LHS true");
x = true;
x ||= returnsBool();
writeln();

writeln("shouldn't short-circuit for ||= with LHS false");
x = false;
x ||= returnsBool();
