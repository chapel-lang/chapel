
use IO;
use List;

class A {
  override proc serialize(writer: fileWriter, ref serializer) throws {
    writer.write("A()");
  }
}

class B {
  override proc serialize(writer: fileWriter, ref serializer) throws {
    writer.write("B()");
  }
}

class C {
  override proc serialize(writer: fileWriter, ref serializer) throws {
    writer.write("C()");
  }
}

class D : C {
  override proc serialize(writer: fileWriter, ref serializer) throws {
    writer.write("D()");
  }
}

proc main() {
  var li : list(owned object);
  li.append(new A());
  li.append(new B());
  li.append(new C());
  li.append(new D());
  writeln(li);
}
