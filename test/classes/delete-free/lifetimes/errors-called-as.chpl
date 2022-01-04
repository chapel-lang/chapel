// Test various successful scenarios of iterating over a linked list (owned)

class clist {
  var elm: int;
  var next: owned clist?;
}

proc showU1(name: string, list) {
  write("showU1 ", name, ":");
  var curr = list: borrowed class?;
  while const cur = curr {
    write(" ", cur.elm);
    curr = cur.next;
  }
  writeln();
}

var c0: owned clist? = nil;
showU1("c0", c0);
