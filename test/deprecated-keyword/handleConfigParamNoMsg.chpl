// Handle when a config param is referenced via its flag
deprecated config param x: bool = false;
config param y: bool = false;

proc main() {
  writeln(x);
}
