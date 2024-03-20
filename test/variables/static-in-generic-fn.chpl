proc newInt() {
  var x: int;
  return x;
}

proc doIt(x: ?t) {
  @functionStatic
  ref xx = newInt();
  xx += x;
  writeln("In ", t:string, ", version, xx is: ", xx);
}

var a: int(32) = 5;
var b: int(64) = 2;

doIt(a);
doIt(a);
doIt(b);
doIt(b);
doIt(a);
doIt(b);
