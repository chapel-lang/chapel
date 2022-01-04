proc retSync(x: ?t): sync t {
  var xx: sync t = x;
  return xx;
}

var x = retSync(3).readFE();
var y: sync int = retSync(4);

writeln(x.type:string, ", ", y.type:string);
