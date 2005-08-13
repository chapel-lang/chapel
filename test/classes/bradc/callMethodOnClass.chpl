class C {
  var x = 1.2;
}

function newC() {
  var c = C();
  c.x = 2.3;
  return c;
}

class D {
  var y: C;

  function start {
    y = newC();
  }

  function testit {
    writeln("y.x is: ", y.x);
  }
}

function main() {
  var myD = D();
  D.start;  -- BUG if I call this on D!!
  D.testit;
}
