// Returning a param is not a good idea because the body of main()
// will not get executed.

proc hi() { writeln("hello"); }

proc main() param {
  hi();
  return 0;
}
