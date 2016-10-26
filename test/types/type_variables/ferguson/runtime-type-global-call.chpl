proc getDom() {
  return {1};
}

type arrType = [getDom()] real;

proc main() {

  var a:arrType;

  // now a.domain should be {1}
  writeln(a.domain);
  writeln(a);
}
