class A {
  var x: int = 0;
}

class A1: A, writeSerializable {
  var y: int = 0;

  override proc serialize(writer, ref serializer) throws {
    writer.write("Hello from class A1!");
  }
}

proc main() {
  var a: A = new A();
  var b: A1 = new A1();
  var c: A = new A1();

  writeln(a);
  writeln(b);
  writeln(c);
}
