
use IO;
use Json;

record R {
  var x : int;
  var y : real;

  proc init(x: int = 0, y: real = 0.0) {
    this.x = x;
    this.y = y;
  }

  proc init(r: fileReader) {
    ref fmt = r.formatter;
    fmt.readTypeStart(r, R);
    this.x = fmt.readField(r, "x", int);
    this.y = fmt.readField(r, "y", real);
    fmt.readTypeEnd(r, R);
  }

  proc equals(other: R) {
    return this.x == other.x &&
           this.y == other.y;
  }
}

record G {
  type A;
  type B;
  var x : A;
  var y : B;

  proc init(type A, type B, x: A = 0, y: B = 0) {
    this.A = A;
    this.B = B;
    this.x = x;
    this.y = y;
  }

  proc init(type A, type B, r: fileReader) {
    this.A = A;
    this.B = B;
    ref fmt = r.formatter;
    fmt.readTypeStart(r, G(A,B));
    this.x = fmt.readField(r, "x", A);
    this.y = fmt.readField(r, "y", B);
    fmt.readTypeEnd(r, G(A,B));
  }

  proc equals(other: G) {
    return this.x == other.x &&
           this.y == other.y;
  }
}

class Parent {
  var x : int;

  proc init(x: int = 0) {
    this.x = x;
  }
  proc init(r: fileReader) {
    ref fmt = r.formatter;
    fmt.readTypeStart(r, Parent);
    this.x = fmt.readField(r, "x", int);
    fmt.readTypeEnd(r, Parent);
  }

  proc encodeTo(w: fileWriter) {
    ref fmt = w.formatter;
    fmt.writeTypeStart(w, this.type);
    fmt.writeField(w, "x", x);
    fmt.writeTypeEnd(w, this.type);
  }

  proc equals(other: borrowed Parent) {
    return this.x == other.x;
  }
}

class Child : Parent {
  var y : real;

  proc init(x: int = 0, y: real = 0.0) {
    super.init(x);
    this.y = y;
  }
  proc init(r: fileReader) {
    ref fmt = r.formatter;
    fmt.readTypeStart(r, Child);
    super.init(r);
    this.y = fmt.readField(r, "y", real);
    fmt.readTypeEnd(r, Child);
  }

  override proc encodeTo(w: fileWriter) {
    ref fmt = w.formatter;
    fmt.writeTypeStart(w, this.type);
    super.encodeTo(w);
    fmt.writeField(w, "y", y);
    fmt.writeTypeEnd(w, this.type);
  }

  proc equals(other: borrowed Child) {
    return this.x == other.x &&
           this.y == other.y;
  }
}

proc test(type FormatWriter, type FormatReader) {
  var f = openmem();
  const rval = new R(5, 42.0);
  const gval = new G(real, int, 5.0, 42);
  const pval = new Parent(5);
  const cval = new Child(5, 42.0);

  proc print(str: string, val) {
    stdout.writeLiteral(str + ": ");
    stdout.withFormatter(FormatWriter).writeln(val);
  }

  proc helper(val, type T) {
    {
      f.writer().withFormatter(FormatWriter).write(val);
      print("wrote", val);
    }
    {
      var read = f.reader().withFormatter(FormatReader).read(T);
      assert(val.equals(read));
      print("got", read);
    }
  }

  helper(rval, R);
  helper(gval, G(real,int));
  helper(pval, owned Parent);
  helper(cval, owned Child);
}

proc main() {
  test(DefaultWriter, DefaultReader);
  test(JsonWriter, JsonReader);
}
