use LinkedLists;

class C {
  var x: int;
}

class D: C {
  var y: int;
}

var s: LinkedList(unmanaged C);

s.append(new unmanaged D(1, 2));
s.append(new unmanaged C(3));

writeln(s);

for c in s do delete c;

s.destroy();
