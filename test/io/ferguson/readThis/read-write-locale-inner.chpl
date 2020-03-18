record A {
  var x:int;
  proc writeThis(writer) throws {
    var loc = writer.readWriteThisFromLocale();
    writeln("in A.writeThis loc=", loc.id);
    writer.write(x);
  }
}
record B {
  var a:A;
  proc writeThis(writer) throws {
    var loc = writer.readWriteThisFromLocale();
    writeln("in B.writeThis loc=", loc.id);
    writer.write(a);
  }
}

on Locales[1] {

  var r = new B(new A(1));
  writeln(r);
}

