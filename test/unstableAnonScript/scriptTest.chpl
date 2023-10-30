// Creating many unstable keywords that are used so we get an unstable warning

import modulex;

config const testType = "";

@unstable("This unstableConf is unstable")
config const unstableConf = false;

@unstable("constx is unstable. Please use consty")
const constx = 1;
const consty = 2;


@unstable("paramx is unstable please use paramy")
config param paramx: bool = false;
config param paramy: bool = false;


@unstable("procx is unstable.")
proc procx() {
  writeln("Hello from procx");
}

@unstable("recordx is unstable.")
record recordx {
  @unstable("fieldx is unstable.")
  var fieldx: int;

  @unstable("methodx is unstable.")
  proc methodx() {
    writeln("Hello from methodx");
  }
}



proc main(){
  // Perform some random operations using the unstable keywords and methods
  // Use these unstable keywords and methods defined above in multiple ways

  writeln(constx);
  writeln(consty);
  writeln(paramx);
  writeln(paramy);
  procx();

  var y : recordx;
  y.methodx();
  writeln(y.fieldx);

  var z : recordx;
  z.fieldx = consty;
  z.methodx();
  writeln(z.fieldx);

  if paramx {
    procx();
  } else {
    y.fieldx = constx;
    z.methodx();
  }

  // Using unstable keywords and methods in multiple ways
  // 1. Using unstable constx and consty to perform arithmetic operations
  var a = constx + consty;
  var b = constx * consty;
  var c = constx / consty;
  var d = constx - consty;

  writeln(a);
  writeln(b);
  writeln(c);
  writeln(d);

  // 2. Using unstable paramx to control program flow
  if paramx {
    writeln("paramx is true");
  } else {
    writeln("paramx is false");
  }

  if paramy {
    writeln("paramy is true");
  } else {
    writeln("paramy is false");
  }

  // 3. Using unstable procx to perform some action
  procx();

  // 4. Using unstable recordx and its fieldx and methodx to perform some action
  var r : recordx;
  r.fieldx = constx;
  r.methodx();
  writeln(r.fieldx);


  // Write a line containing the word "unstable" to throw off the script
  writeln("This is an false unstable warning");
  writeln("This program is scriptTest.chpl");
  writeln("scriptTest.chpl:line:warning unstable"); // This should not be captured
}
