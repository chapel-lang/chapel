{
record myRec {
  var myRecField: int;
}

iter myRec.myIter() {
  foreach i in 1..10 do
    yield (myRecField,i);
}

var r: myRec;
var myRange = 1..10;

foreach i in r.myIter() do writeln(i);
}

writeln();

{
record myRec {
  var myRecField: int;
}

iter myRec.myIter(aRange) {
  foreach i in aRange do
    yield (myRecField,i);
}

var r: myRec;
var myRange = 1..10;

foreach i in r.myIter(myRange) do writeln(i);
}

writeln();

{
record myRec {
  var myRecField: int;
}

iter myRec.myIter(aRange) {
  foreach i in aRange do
    yield (myRecField,i);
}

iter foo() {
  var r: myRec;
  var myRange = 1..10;

  foreach i in r.myIter(myRange) do yield i;
}

foreach i in foo() do writeln(i);
}
