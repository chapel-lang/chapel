record history_real : writeSerializable {
  param size : int;
  var h : size*real;
  var f : real;

  proc ref add(r : real) {
    for i in 1..size-1 by -1 do
      h(i) = h(i-1);
    h(0) = f;
    f = r;
  }

  proc init=(r : real) {
    this.size = this.type.size;
    init this;
    add(r);
  }

  proc init=(other : history_real(?)) {
    this.size = other.size;
    this.h = other.h;
    this.f = other.f;
  }
}

operator =(ref x : history_real(?), y : real) {
  x.add(y);
}

operator =(ref x : history_real(?), y : x.type) {
  x.h = y.h;
  x.f = y.f;
}
operator :(y: real, type t: history_real(?)) {
  var tmp: t = y;
  return tmp;
}

operator >(x : history_real(?), y : real) {
  return x.f > y;
}

operator +(x : history_real(?), y : real) {
  return x.f + y;
}

proc history_real.serialize(writer, ref serializer) throws {
  writer.write(f, " (");
  for i in 0..#size do
    writer.write(h(i), if i < size-1 then ", " else ")");
}
