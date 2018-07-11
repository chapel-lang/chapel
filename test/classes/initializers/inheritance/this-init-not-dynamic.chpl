//
// Calls to this.init() should not be dynamically dispatched.
//
// This is essential to ensure that call to "convenience" initializers
// do not get overridden.  This special case required an extension to
// insertDynamicDispatchCalls() and this test locks that in.
//

class Base {
  proc init() {
    writeln('Base.init()');
    this.init(10);
  }

  // *** A base convenience initializer
  proc init(x : int) {
    writeln('Base.init(x)');
    init(x, x);
  }

  proc init(x : int, y : int) {
    writeln('Base.init(x, y)');
  }
}

class Derived : Base {
  proc init() {
    writeln('Derived.init()');
    init(20);
  }

  // *** A possible override to the base convenience initializer
  proc init(x : int) {
    writeln('Derived.init(', x, ')');
  }
}

var b = new unmanaged Base();

writeln();

var d = new unmanaged Derived();

writeln();
writeln(b);
writeln(d);

delete d;
delete b;
