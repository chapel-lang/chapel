use LinkedLists;

class C {
  var x: int;
}

var s: LinkedList(unmanaged C);

writeln(s);

s.append(new unmanaged C(2));

writeln(s);

for c in s do delete c;

s.destroy();

