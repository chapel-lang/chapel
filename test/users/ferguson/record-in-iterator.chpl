
use LinkedLists;

iter myiter(N /* adding :int makes it work */) {

  /* Moving record myrecord outside of myiter makes it work */
  record myrecord {
    var x:int;
    proc add(y: int) {
      x += y;
    }
  }


  var li: LinkedList(myrecord);

  var r = new myrecord(0);
  var r2 = new myrecord(2);
  r.add(2);
  li.append(r);

  for i in 1..N {
    r2 = new myrecord(i);
    r2.add(i);
    li.append(r2);
  }

  for r in li {
    yield r.x;
  }
}

config const N = 10;

for i in myiter(N) {
  writeln(i);
}
