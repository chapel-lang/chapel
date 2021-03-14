use PeekPoke;

proc declareAndTestAtomicBool() {
  var a: atomic bool;
  writeln(a.peek());
  a.poke(true);
  writeln(a.peek());
}

proc declareAndTestAtomicT(type basetype) {
  var a: atomic basetype;
  writeln(a.peek());
  a.poke(1);
  writeln(a.peek());

}

/* Test peek/poke for all types */
declareAndTestAtomicBool();

declareAndTestAtomicT(int(8));
declareAndTestAtomicT(int(16));
declareAndTestAtomicT(int(32));
declareAndTestAtomicT(int(64));
declareAndTestAtomicT(int);

declareAndTestAtomicT(uint(8));
declareAndTestAtomicT(uint(16));
declareAndTestAtomicT(uint(32));
declareAndTestAtomicT(uint(64));
declareAndTestAtomicT(uint);

declareAndTestAtomicT(real(32));
declareAndTestAtomicT(real(64));
declareAndTestAtomicT(real);

/* Test promotion for atomic int */
var AInt: [1..3] atomic int;
var IInt: [1..3] int;
IInt = AInt.peek(); writeln(IInt);
AInt.poke(1);
IInt = AInt.peek(); writeln(IInt);
