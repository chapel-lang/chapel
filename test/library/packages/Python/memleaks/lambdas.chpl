use Python, List;

proc main() {

  var interp = new Interpreter();

  var l = new Function(interp, "lambda x,: [x] + [x]");
  writeln(l(list(int), 1));
  writeln(l(list(string), "hi"));

  // this has a an extra lis
  writeln(l(list(list(int)), [1, 2, 3]));
}
