use Heap;

class C { var x: int = 0; }
proc C.deinit() { writeln("deinit: " + x:string); }

proc <(left: C, right: C) {
  return left.x < right.x;
}
proc >(left: C, right: C) {
  return left.x > right.x;
}

proc test() {
  var hp1 = new heap((shared C, shared C));
  var hp2 = new heap((shared C, shared C));
  var hp3 = new heap(shared C);

  for i in 1..8 {
    var item = (new shared C(i), new shared C(-i));
    hp1.push(item);
  }

  writeln(hp1.size:string);
  for item in hp1 do writeln(item);
  writeln('---');

  while !hp1.isEmpty() {
    hp2.push(hp1.top());
    hp1.pop();
  }

  writeln(hp1.size:string);
  writeln(hp2.size:string);
  for item in hp2 do writeln(item);

  while !hp2.isEmpty() {
    var (a, b) = hp2.top();
    hp2.pop();
    hp3.push(a);
    hp3.push(b);
  }

  writeln('---');
  writeln(hp3.size);
  for item in hp3 do writeln(item);

  // Purely for side effect of seeing deinitializers fire.
  writeln('---');
  while !hp3.isEmpty() do hp3.pop();

  return;
}
test();

