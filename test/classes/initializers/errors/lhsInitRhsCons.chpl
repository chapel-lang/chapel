class hasInit {
  var x: int;

  proc init(xVal) {
    x = xVal;
  }
}

class hasConstruct {
  var x: int;

  proc hasConstruct(xVal) {
    x = xVal;
  }
}


var hi: hasInit = new hasConstruct(10);
writeln(hi);
delete hi;
