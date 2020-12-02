record R {
  var x:int = 0;
  proc deinit() {
    assert(x == 0);
    x = 99;
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
  r;
  writeln("middle");
}
t3();

proc t4() {
  writeln("t4");
  var r: R;
  {
    writeln("begin inner");
    r;
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
  writeln("end outer");
}
t9();

proc t10() {
  writeln("t10");
  var r: R;
  forall i in 1..2 {
    r;
    writeln("end loop iter");
  }
  writeln("end outer");
}
t10();

proc t10a() {
  writeln("t10a");
  var r: R;
  forall i in 1..2 {
    r;
    writeln("end loop iter");
  }
  writeln(r);
  writeln("end outer");
}
t10a();

proc t10b() {
  writeln("t10b");
  var r: R;
  ref rr = r;
  forall i in 1..2 {
    r;
    writeln("end loop iter");
  }
  writeln(r);
  writeln("end outer");
}
t10b();

proc t11() {
  writeln("t11");
  var r: R;
  coforall i in 1..2 {
    r;
    writeln("end loop iter");
  }
  writeln("end outer");
}
t11();

proc t11o() {
  writeln("t11o");
  var r: R;
  coforall i in 1..2 {
    on Locales[numLocales-1] {
      r;
      writeln("end loop iter");
    }
  }
  writeln("end outer");
}
t11o();

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
  writeln("end outer");
}
t12();

proc t12o() {
  writeln("t12o");
  var r: R;
  var i = 1;
  cobegin with (ref i) {
    on Locales[numLocales-1] {
      r;
      writeln("end inner");
    }
    {
      i = 2;
    }
  }
  writeln("end outer");
}
t12o();

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

proc t17() {
  writeln("t17");
  var r: R;
  var i = 1;
  sync {
    begin {
      r;
      writeln("end inner");
    }
  }
  writeln("end outer");
}
t17();

proc t18() {
  writeln("t18");
  var r: R;
  var done:sync int;

  begin {
    writeln("task one ", r);
    done = 1;
  }

  done;

  writeln("end outer");
}
t18();

proc t18o() {
  writeln("t18o");
  var r: R;
  var done:sync int;

  begin on Locales[numLocales-1] {
    writeln("task one ", r);
    done = 1;
  }

  done;

  writeln("end outer");
}
t18o();

proc t20() {
  writeln("t20");
  var r: R;
  var step1:sync int;
  var step2:sync int;

  begin {
    step1; // wait for 1st task
    writeln("task one ", r);
    step2 = 1;
  }

  begin {
    writeln("task two ", r);
    step1 = 1;
  }

  step2;

  writeln("end outer");
}
t20();

proc makeR() {
  writeln("in makeR");
  return new R();
}

proc t21() {
  writeln("t21");
  var r = if option then makeR() else makeR();
  writeln("end outer");
}
t21();

proc t21a() {
  writeln("t21a");
  const ref r = if option then makeR() else makeR();
  writeln("end outer");
}
t21a();

proc t21b() {
  writeln("t21b");
  const ref r = if !option then makeR() else makeR();
  writeln("end outer");
}
t21b();

proc t21c() {
  writeln("t21c");
  const ref r = if true then makeR() else makeR();
  writeln("end outer");
}
t21c();

proc t21d() {
  writeln("t21d");
  const ref r = if false then makeR() else makeR();
  writeln("end outer");
}
t21d();

proc t21e() {
  writeln("t21e");
  var r = if true then makeR() else makeR();
  writeln("end outer");
}
t21e();

proc t21f() {
  writeln("t21f");
  var r = if false then makeR() else makeR();
  writeln("end outer");
}
t21f();

proc t22() {
  writeln("t22");
  {
    writeln("begin inner");
    var r: R;
    writeln("end inner");
  }
  writeln("end outer");
}
t22();

proc t23() {
  writeln("t23");
  {
    writeln("begin inner");
    var r: R;
    ref rr = r;
    writeln("end inner");
  }
  writeln("end outer");
}
t23();

proc t24() {
  writeln("t24");
  {
    {
      writeln("begin inner");
      var r: R;
      ref rr = r;
      writeln("end inner");
    }
  }
  writeln("end outer");
}
t24();

proc t25() {
  writeln("t25");
  {
    {
      writeln("begin inner");
      var r: R;
      writeln("end inner");
    }
  }
  writeln("end outer");
}
t25();

proc t26() {
  writeln("t26");
  var r: R;
  writeln(r.type:string);
}
t26();

proc t27() {
  writeln("t27");
  var r: R;
  writeln(r.type:string);
  writeln("end");
}
t27();

proc t28() {
  writeln("t28");
  var r: R;
  sync {
    {
      begin {
        writeln("begin inner");
        r;
        writeln("end inner");
      }
    }
  }
  writeln("end outer");
}
t28();

proc t29() {
  writeln("t29");
  var r: R;
  sync {
    if option {
      begin {
        writeln("begin inner");
        r;
        writeln("end inner");
      }
    }
  }
  writeln("end outer");
}
t29();

proc g(arg) { return arg; }
proc h(a, b) { }

proc t30() {
  writeln("t30");
  g(makeR());
  writeln("end outer");
}
t30();

proc t30a() {
  writeln("t30a");
  if option then
    g(makeR());
  else
    g(makeR());
  writeln("end outer");
}
t30a();

proc t30b() {
  writeln("t30b");
  g(if option then makeR() else makeR());
  writeln("end outer");
}
t30b();

proc t30c() {
  writeln("t30c");
  g(makeR());
  {
    writeln("nested block");
  }
  writeln("end outer");
}
t30c();

proc t31() {
  writeln("t31");
  g(g(makeR()));
  writeln("end outer");
}
t31();

proc t32() {
  writeln("t32");
  h(g(makeR()), makeR());
  writeln("end outer");
}
t32();

proc t33() {
  writeln("t33");
  var x = g(g(makeR()));
  writeln("end outer");
}
t33();

proc t33a() {
  writeln("t33a");
  var x = if option then g(makeR()) else g(makeR());
  writeln("end outer");
}
t33a();


proc t34() {
  writeln("t34");
  const x = g(g(makeR()));
  writeln("end outer");
}
t34();

proc t35() {
  writeln("t35");
  const ref x = g(g(makeR()));
  writeln("end outer");
}
t35();

writeln("t36");
g(makeR());
writeln("end t36");

writeln("t37");
var t37glob = g(makeR());
writeln("end t37");
