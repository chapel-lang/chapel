
var count: int;
//module count {
cobegin {

  atomic {
    writeln("SS: ATOMIC1");
    count += 1;
  }
  atomic {
    writeln("SS: ATOMIC2");
    count += 1;
  }
  atomic {
    writeln("SS: ATOMIC3");
    count += 1;
  }
  atomic {
    writeln("SS: ATOMIC4");
    count += 1;
  }
}

sync {
  writeln("SS: ATOMIC5");
  begin atomic {count += 1; }
  begin atomic {count += 1; }
  begin atomic {count += 1; }
}

writeln("Final Value:", count);

