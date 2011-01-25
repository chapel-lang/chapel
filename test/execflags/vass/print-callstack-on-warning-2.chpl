// generate compiler warnings when some calls are created by the compiler

var mi1 = new myiter1();
for i in mi1 do writeln(i);
test1();

class myiter1 {
  iter these() {
    compilerWarning("warning from myiter1.these(), depth=0", 0);
    compilerWarning("warning from myiter1.these(), default depth");
    yield 111;
  }
}

proc test1() {
  for i in mi1 do writeln(i);
}

/////////////////////////////////

var mi2 = new myiter2();
for i in mi2 do writeln(i);
test2();

class myiter2 {
  iter these() {
    compilerWarning("warning from myiter2.these(), depth=0", 0);
    compilerWarning("warning from myiter2.these(), default depth");
    yield 222;
  }
}

proc test2() {
  for i in mi2 do writeln(i);
}
