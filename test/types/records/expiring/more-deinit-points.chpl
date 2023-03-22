record R {
  proc deinit() {
    writeln("R.deinit()");
  }
}

proc returnAnR() {
  return new R();
}

proc returnArrayOfR() {
  var A:[1..1] R = for i in 1..1 do returnAnR();
  return A;
}

proc g(arg) { return 1; }
proc h(arg) { return returnAnR(); }

proc notSplitInit() {
  writeln("starting notSplitInit");
  var x = g(returnAnR()); // currently, the R is deinited at end of block
  writeln("ending notSplitInit");
}
notSplitInit();
writeln();

proc splitInit() {
  writeln("starting splitInit");
  var x;
  x = g(returnAnR()); // currently, the R is deinited at end of block
  writeln("ending splitInit");
}
splitInit();
writeln();

proc assign() {
  var x = 1;
  writeln("starting assign");
  x = g(returnAnR()); // currently, the R is deinited just after this statement
  writeln("ending assign");
}
assign();
writeln();

proc notInit() {
  writeln("starting notInit");
  g(returnAnR()); // currently, the R is deinited just after this statement
  writeln("ending notInit");
}
notInit();
writeln();

proc refInit() {
  writeln("starting refInit");
  const ref rr = g(returnAnR());
  writeln("ending refInit");
}
refInit();
writeln();

proc inArrayType() {
  writeln("starting inArrayType");
  var A:[1..g(returnAnR())] int; 
  writeln("ending inArrayType");
}
inArrayType();
writeln();

proc inArrayElt() {
  writeln("starting inArrayElt");
  g(returnArrayOfR());
  writeln("ending inArrayElt");
}
inArrayElt();
writeln();

proc inArrayElt2() {
  writeln("starting inArrayElt2");
  var A:[1..1] int = g(returnAnR());
  writeln("ending inArrayElt2");
}
inArrayElt2();
writeln();

proc refInitNested() {
  writeln("starting refInitNested");
  const ref rr = g(h(returnAnR())) ;
  writeln("ending refInitNested");
}
refInitNested();
writeln();

proc notInitNested() {
  writeln("starting notInitNested");
  g(h(returnAnR()));
  writeln("ending notInitNested");
}
notInitNested();
writeln();
