// Handle when a config var is referenced via its flag
deprecated "don't use x, use y" config var x: bool = false;
config var y: bool = false;

proc main() {
  writeln(x);
}
