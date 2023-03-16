var globalOwnBar = new owned bar(x = 12, y = 13);
var globalBar : borrowed bar = globalOwnBar.borrow();
writeln(globalBar);


class foo {
  type t;
  var x : t = globalBar;
  proc print() {
    writeln(x);
  }
}

class bar {
  var x : int;
  var y : int;
}

var fOwn = new owned foo(t=borrowed bar);
var f : borrowed foo(t=borrowed bar) = fOwn.borrow();

writeln(f);
f.print();
