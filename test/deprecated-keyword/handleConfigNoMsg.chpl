// Handle when a config var is referenced via its flag
deprecated config var x: bool = false;
config var y: bool = false;

proc main() {
  writeln(x);
}
