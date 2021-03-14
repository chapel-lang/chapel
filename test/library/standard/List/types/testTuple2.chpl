import List.list;

class C { var x: int = 0; }

proc test() {
  var lst: list((shared C, shared C));

  for i in 1..8 {
    var item = (new shared C(i), new shared C(-i));
    lst.append(item);
  }

  writeln(lst.size:string);

  for item in lst do writeln(item);

  while !lst.isEmpty() {
    var item = lst.pop();
    writeln(item);
  }

  return;
}
test();

