// Adapted from user submitted code in GitHub issue #6642

class SuperPrinter {
  //for faulty behavior this formal must have an explicit ref intent
  proc print(ref data) {
    writeln("SuperPrinter: ", data.x);
  }
}

class SubPrinter : SuperPrinter {
  proc print(ref data) {
    writeln("Subprinter: ", data.x);
  }
}

class Foo { var x = 10; }

proc main() {
  var printer: SuperPrinter;
  printer = new SubPrinter();

  //
  // At the time this test was created, IWR was widening all formals in
  // virtual methods but was failing to widen the corresponding actuals. This
  // resulted in a compile-time failure when the backend compiler encountered
  // the type mismatch between a narrow/local actual and a wide formal.
  //
  var data = new Foo();
  printer.print(data);

  delete data, printer;
}
