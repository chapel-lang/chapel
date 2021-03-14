config param loopform = 1;

record R {
  var x = 42;

  proc init=(other: R) {
    writeln("In copy init");
    this.x = other.x + 1;
  }
}

proc =(ref lhs: R, rhs: R) {
  writeln("In assignment");
  lhs.x = rhs.x + 5;
}

var tup = (1, 2.3, "hi", new R());

if loopform == 1 then
  for (t,i) in zip(tup, 1..) do
    writeln((i,t));

if loopform == 2 then
  for (i,t) in zip(1..4, tup) do
    writeln((i,t));

if loopform == 3 then
  coforall (t,i) in zip(tup, 1..) do
    writeln((i,t));

if loopform == 4 then
  coforall (i,t) in zip(1..4, tup) do
    writeln((i,t));

if loopform == 5 then
  forall (t,i) in zip(tup, 1..) do
    writeln((i,t));

if loopform == 6 then
  forall (i,t) in zip(1..4, tup) do
    writeln((i,t));

if loopform == 7 then
  for ind in zip(tup, 1..) do
    writeln(ind);

if loopform == 8 then
  for ind in zip(1..4, tup) do
    writeln(ind);
