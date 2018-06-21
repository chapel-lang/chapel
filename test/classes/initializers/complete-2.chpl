class MyClass0 {
  proc init() {
    writeln('MyClass0.init');
  }
}

class MyClass1 : MyClass0 {
  const x : int = 10;
  const y : int = 20;

  proc init(val : int) {
    writeln('MyClass1.init');
    y = val;

    complete();
  }
}

proc main() {
  var c : unmanaged MyClass1 = new unmanaged MyClass1(50);

  writeln(c);

  delete c;
}
