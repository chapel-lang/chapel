
use IO, Reflection;

enum Variants { A, B, C, CG, D }
enum Modes { Write, Read, Init }

config param variant : Variants = Variants.A;
config param mode : Modes = Modes.Write;

// Presence of normal, non-deserializing initializer should not disable
// generation of serialize/deserialize.
record A {
  var x : int;

  proc init(x : int = 0) {
    this.x = x;
  }
}

// Should disable generation of 'deserialize' and deserializing 'init'
record B : writeSerializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    ChapelIO.serializeDefaultImpl(writer, serializer, this);
  }
}

// Should disable generation of 'serialize' and 'deserialize'
record C : initDeserializable {
  var x : int;

  proc init(x : int = 0) {
    this.x = x;
  }
  proc init(reader, ref deserializer) {
    init this;
    ChapelIO.deserializeDefaultImpl(reader, deserializer, this);
  }
}

// Should also disable generation of 'serialize' and 'deserialize', but
// involves generics for completeness.
record CG : initDeserializable {
  type T;
  var x : T;

  proc init(type T, x : T = 0) {
    this.T = T;
    this.x = x;
  }
  proc init(type T, reader, ref deserializer) {
    this.T = T;
    init this;
    ChapelIO.deserializeDefaultImpl(reader, deserializer, this);
  }
}

// Should disable generation of 'serialize' and deserializing 'init'
record D : readDeserializable {
  var x : int;

  proc ref deserialize(reader, ref deserializer) throws {
    ChapelIO.deserializeDefaultImpl(reader, deserializer, this);
  }
}

proc getType() type {
  select variant {
    when Variants.A do return A;
    when Variants.B do return B;
    when Variants.C do return C;
    when Variants.CG do return CG(int);
    when Variants.D do return D;
    otherwise do return nothing;
  }
}

proc doWrite(f, val) {
  f.writer(locking=false).write(val);
}

proc doDeserialize(f, val) {
  f.writer(locking=false).write("(x = ", val.x, ")");
  var x : val.type;
  f.reader(locking=false).read(x);
  assert(x.x == 10);
}

proc doInit(f, val) {
  f.writer(locking=false).write("(x = ", val.x, ")");
  var x = f.reader(locking=false).read(val.type);
  assert(x.x == 10);
}

proc main() {
  var f = openMemFile();

  var val : getType();
  val.x = 10;

  select mode {
    when Modes.Write do doWrite(f, val);
    when Modes.Read do doDeserialize(f, val);
    when Modes.Init do doInit(f, val);
  }
}
