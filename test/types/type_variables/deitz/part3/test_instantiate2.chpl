var globalBar : borrowed bar        = (new owned bar(x = 12, y = 13)).borrow();
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

var f : borrowed foo(t=borrowed bar) = (new owned foo(t=borrowed bar)).borrow();

writeln(f);
f.print();
