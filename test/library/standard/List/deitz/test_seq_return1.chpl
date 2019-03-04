use LinkedLists;

proc f() {
  var s : LinkedList(int);
  var i : int;
  while i < 10 {
    s.append(i*4);
    i += 1;
  }
  return s;
}

var l = f();

writeln(l);

l.destroy();


