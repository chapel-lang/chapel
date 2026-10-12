config param skipFor = false;

proc foo(x: _iteratorRecord) {
  var arr = [val in x] val;
  writeln(arr);
}

foo(forall i in 1..10 do i);
foo([i in 1..10] i);
if !skipFor then
  foo(for i in 1..10 do i);
