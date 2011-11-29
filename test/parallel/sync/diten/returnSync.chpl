proc retSync(x: ?t): sync t {
  var xx: sync t = x;
  return xx;
}

var x = retSync(3);
var y: sync int = retSync(4);

writeln((typeToString(x.type), typeToString(y.type)));
