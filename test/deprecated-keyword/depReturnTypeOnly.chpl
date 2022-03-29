config param fooReturnsBool = false;

deprecated "foo returning 'int' is deprecated, please compile with '-sfooReturnsBool=true' to get the new return type"
proc foo(): int where !fooReturnsBool {
  writeln("In old foo");
  return 0;
}

proc foo(): bool where fooReturnsBool {
  writeln("In new foo");
  return false;
}

proc main() {
  foo();
}
