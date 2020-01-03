record R {
  proc deinit() {
    writeln("deinit");
  }
}

config const option = true;

proc t1() {
  writeln("t1");
  var r: R;
}
t1();

proc t2() {
  writeln("t2");
  var r: R;
  var i: int;
  writeln("middle");
  r;
}
t2();

proc t3() {
  writeln("t3");
  var r: R;
  var i: int;
  writeln("middle");
}
t3();

proc t4() {
  writeln("t4");
  var r: R;
  {
    writeln("begin inner");
    r;
    // TODO: destroy it here 
    writeln("end inner");
  }
  writeln("end outer");
}
t4();

proc t5() {
  writeln("t5");
  var r: R;
  if option {
    r;
    writeln("end if");
  } else {
    r;
    writeln("end else");
  }
  // TODO: destroy it here
  writeln("end outer");
}
t5();

proc t6() {
  writeln("t6");
  var r: R;
  if option {
    r;
    writeln("end if");
  }
  // TODO: destroy it here
  writeln("end outer");
}
t6();

proc t7() {
  writeln("t7");
  var r: R;
  for i in 1..2 {
    r;
    writeln("end loop iter");
  }
  // TODO: destroy it here
  writeln("end outer");
}
t7();

proc t8() {
  writeln("t8");
  var r: R;
  var i = 1;
  while i < 2 {
    r;
    i += 1;
    writeln("end loop iter");
  }
  // TODO: destroy it here
  writeln("end outer");
}
t8();

proc t9() {
  writeln("t9");
  var r: R;
  var i = 1;
  do {
    r;
    i += 1;
    writeln("end loop iter");
  } while i < 2;
  // TODO: destroy it here
  writeln("end outer");
}
t9();

proc t10() {
  writeln("t10");
  var r: R;
  // TODO: bug - it is destroyed here
  forall i in 1..2 {
    r;
    writeln("end loop iter");
  }
  writeln("end outer");
}
t10();

proc t11() {
  writeln("t11");
  var r: R;
  coforall i in 1..2 {
    r;
    writeln("end loop iter");
  }
  // TODO: destroy it here
  writeln("end outer");
}
t11();

proc t12() {
  writeln("t12");
  var r: R;
  var i = 1;
  cobegin with (ref i) {
    {
      r;
      writeln("end inner");
    }
    {
      i = 2;
    }
  }
  // TODO: destroy it here
  writeln("end outer");
}
t12();

proc t13() {
  writeln("t13");
  var r: R;
  on Locales[numLocales-1] {
    r;
    writeln("end inner");
  }
  writeln("end outer");
}
t13();

proc t14() {
  writeln("t14");
  var r: R;
  {
    writeln("begin inner");
    {
      writeln("begin inner inner");
      r;
    }
    writeln("end inner");
  }
  writeln("end outer");
}
t14();

proc t15() {
  writeln("t15");
  {
    var r: R;
    writeln("begin inner");
    {
      writeln("begin inner inner");
      r;
    }
    writeln("end inner");
  }
  writeln("end outer");
}
t15();

proc t16() {
  writeln("t16");
  {
    var r: R;
    writeln("begin inner");
    {
      writeln("begin inner inner");
      r;
      writeln("end inner inner");
    }
    writeln("end inner");
  }
  writeln("end outer");
}
t16();
