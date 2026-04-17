use Python;

var interp = new Interpreter();

var tup = interp.get("tuple")(owned PyTuple, [1, 2, 3, 4, 5]);
writeln("yielding Values from tuple");
for i in tup {
  writeln(i);
}
writeln("yielding ints from tuple");
for i in tup.these(int) {
  writeln(i);
}
