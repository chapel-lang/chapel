class C {

  // With an explicit method name and function call, this works:
  //  proc foo() {
  // With the initialize method, it doesn't -- why?
  proc initialize() {
    const blo = if (here.id == 0) then 333 else 444;
    writeln("blo is: ", blo);
  }
}

var myC = new C();
//myC.foo();
delete myC;
