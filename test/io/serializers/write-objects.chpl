
use IO;
use List;

class A : writeSerializable {
  override proc serialize(writer: fileWriter(?), ref serializer) throws {
    writer.write("A()");
  }
}

class B : writeSerializable {
  override proc serialize(writer: fileWriter(?), ref serializer) throws {
    writer.write("B()");
  }
}

class C : writeSerializable {
  override proc serialize(writer: fileWriter(?), ref serializer) throws {
    writer.write("C()");
  }
}

class D : C, writeSerializable {
  override proc serialize(writer: fileWriter(?), ref serializer) throws {
    writer.write("D()");
  }
}

proc main() {
  var li : list(owned RootClass);
  li.pushBack(new A());
  li.pushBack(new B());
  li.pushBack(new C());
  li.pushBack(new D());
  writeln(li);
}
