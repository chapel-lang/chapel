use Python;

proc main() {
  var interp = new Interpreter();

  writeln("importing torch module");
  var torch = interp.importModule("torch");

  writeln("getting tensor constructor");
  var tensor = torch.get("tensor");

  writeln("creating tensor");
  var t = tensor([[1.0, 2.0], [3.0, 4.0]]);

  writeln(t);
}
