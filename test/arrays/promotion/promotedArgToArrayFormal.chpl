record C { 
  var x = 10;
  proc getX() {
    return x;
  }
}

var cs: [1..1] C;
bar(cs.getX());

proc bar(arrayArg: [] int) {
  writeln(arrayArg);
}
