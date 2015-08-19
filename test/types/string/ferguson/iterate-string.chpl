var A = ["this", "is", "a", "test"];

iter myiter() : string {
  for a in A do yield a;
}

proc main() {

  var baz: string;

  for x in myiter() {
    writeln(x);
  }
}

