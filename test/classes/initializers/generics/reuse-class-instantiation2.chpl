class MyClass {
  var x;
  var y;

  proc init(_x : int, _y : int) {
    writeln('Concrete');

    x = _x;
    y = _y;

  }

  proc init(_x, _y) {
    writeln('Generic');

    x = _x;
    y = _y;

  }
}



proc main() {
  var c1 = new unmanaged MyClass(10, 20);
  var c2 = new unmanaged MyClass(20, 30);
  var c3 = new unmanaged MyClass(30, 40);

  var c5 = new unmanaged MyClass(false, 40.0);
  var c6 = new unmanaged MyClass( true, 50.0);
  var c7 = new unmanaged MyClass( true, 60.0);

  writeln('c1 ', c1);
  writeln('c2 ', c2);
  writeln('c3 ', c3);

  writeln('c5 ', c5);
  writeln('c6 ', c6);
  writeln('c7 ', c7);

  delete c7;
  delete c6;
  delete c5;

  delete c3;
  delete c2;
  delete c1;
}
