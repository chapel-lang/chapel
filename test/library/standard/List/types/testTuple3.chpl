import List.list;

class C { var x: int = 0; }
proc C.deinit() { writeln("deinit: " + x:string); }

proc test() {
  var lst1: list((shared C, shared C));
  var lst2: list((shared C, shared C));
  var lst3: list(shared C);

  for i in 1..8 {
    var item = (new shared C(i), new shared C(-i));
    lst1.append(item);
  }

  writeln(lst1.size:string);
  for item in lst1 do writeln(item);
  writeln('---');

  while !lst1.isEmpty() {
    lst2.append(lst1.pop());
  }

  writeln(lst1.size:string);
  writeln(lst2.size:string);
  for item in lst2 do writeln(item);

  while !lst2.isEmpty() {
    var (a, b) = lst2.pop();
    lst3.append(a);
    lst3.append(b);
  }

  writeln('---');
  writeln(lst3.size);
  for item in lst3 do writeln(item);

  // Purely for side effect of seeing deinitializers fire.
  writeln('---');
  while !lst3.isEmpty() do lst3.pop();

  return;
}
test();

