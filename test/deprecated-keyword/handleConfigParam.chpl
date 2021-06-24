// Handle when a config param is referenced via its flag
deprecated "don't use x, use y" config param x: bool = false;
config param y: bool = false;

proc main() {
  writeln(x);
}
