config const upper = true;

iter yieldDomains() {

  for i in 1..3 {
    if upper {
      var D = {i..10};
      yield D;
    } else {
      var D = {1..i};
      yield D;
    }
  }
}
iter yieldTupleDomains1() {

  for i in 1..3 {
    if upper {
      var D1 = {i..10};
      yield (D1,);
    } else {
      var D1 = {1..i};
      yield (D1,);
    }
  }
}
iter yieldTupleDomains2() {

  for i in 1..3 {
    if upper {
      var D1 = {i..10};
      var D2 = {i..11};
      yield (D1, D2);
    } else {
      var D1 = {1..i};
      var D2 = {0..i};
      yield (D1, D2);
    }
  }
}


proc run() {

  writeln("yieldDomains");
  for D in yieldDomains() {
    writeln(D);
  }
  writeln("yieldTupleDomains1");
  for (D,) in yieldTupleDomains1() {
    writeln(D);
  }
  writeln("yieldTupleDomains2");
  for (D1,D2) in yieldTupleDomains2() {
    writeln(D1, " ", D2);
  }

}

run();
