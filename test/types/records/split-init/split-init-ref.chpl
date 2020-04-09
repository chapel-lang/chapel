config const cond = false;

proc printRef(name, ref arg) {
  writeln(name, ": ", arg.type:string, " = ", arg);
}
proc printConstRef(name, const ref arg) {
  writeln(name, ": ", arg.type:string, " = ", arg);
}

proc test() {
  var x:int = 0;

  ref yes1;
  yes1 = x;
  x = 1;
  printRef("yes1", yes1);

  const ref yes2;
  {
    x = 24;
    yes2 = 2;
  }
  printConstRef("yes2", yes2);

  const ref yes3;
  {
    if cond {
      yes3 = 123;
    } else {
      yes3 = x;
      x = 3;
    }
    printConstRef("yes3", yes3);
  }

  const ref yes4;
  {
    if !cond {
      yes4 = 4;
    } else {
      yes4 = x;
      x = 124;
    }
    printConstRef("yes4", yes4);
  }


  ref yes5:int;
  yes5 = x;
  x = 5;
  printRef("yes5", yes5);
}
test();
