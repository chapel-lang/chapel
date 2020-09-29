const colWidth = 20;
const fmt = ("%-"+colWidth:string+"s|")*3+"\n";

proc writeHeader(header) {
  writeln();
  writeln();
  writeln(header);
  writeln();
  writef(fmt, "arg", "definedConst", "tracks arrays?");
  writef(fmt, "-"*colWidth, "-"*colWidth, "-"*colWidth);
}

proc test(name, r) {
  writef(fmt, name, r.d.definedConst, r.d._value._arrs.size == 1);
}

proc testAll(type t) {
  {
    var r = new t({1..3});
    test("literal", r);
  }

  {
    var d = {1..3};
    var r = new t(d);
    test("var", r);
  }

  {
    const d = {1..3};
    var r = new t(d);
    test("const", r);
  }

  {
    var r = new t(returnLiteral());
    test("literal return", r);
  }

  {
    var r = new t(returnVar());
    test("var return", r);
  }

  {
    var r = new t(returnConst());
    test("const return", r);
  }
}

proc returnLiteral() {
  return {1..3};
}

proc returnConst() {
  const d = {1..3};
  return d;
}

proc returnVar() {
  var d = {1..3};
  return d;
}

{
  writeHeader("Generic var field, default initializer");

  record R {
    var d;
    var a: [d] int;
  }

  testAll(R);
}

{
  writeHeader("Generic const field, default initializer");

  record R {
    const d;
    var a: [d] int;
  }

  testAll(R);
}

{
  writeHeader("Concrete var field, default initializer");

  record R {
    var d: domain(1, int, false);
    var a: [d] int;
  }

  testAll(R);
}

{
  writeHeader("Concrete const field, default initializer");

  record R {
    const d: domain(1, int, false);
    var a: [d] int;
  }

  testAll(R);
}

{
  writeHeader("Generic var field, generic user initializer with default intent");

  record R {
    var d;
    var a: [d] int;

    proc init(d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic var field, generic user initializer with in intent");

  record R {
    var d;
    var a: [d] int;

    proc init(in d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic var field, generic user initializer with const in intent");

  record R {
    var d;
    var a: [d] int;

    proc init(const in d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete var field, generic user initializer with default intent");

  record R {
    var d: domain(1, int, false);
    var a: [d] int;

    proc init(d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete var field, generic user initializer with in intent");

  record R {
    var d: domain(1, int, false);
    var a: [d] int;

    proc init(in d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete var field, generic user initializer with const in intent");

  record R {
    var d: domain(1, int, false);
    var a: [d] int;

    proc init(const in d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic var field, concrete user initializer with default intent");

  record R {
    var d;
    var a: [d] int;

    proc init(d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic var field, concrete user initializer with in intent");

  record R {
    var d;
    var a: [d] int;

    proc init(in d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic var field, concrete user initializer with const in intent");

  record R {
    var d;
    var a: [d] int;

    proc init(const in d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete var field, concrete user initializer with default intent");

  record R {
    var d: domain(1, int, false);
    var a: [d] int;

    proc init(d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete var field, concrete user initializer with in intent");

  record R {
    var d: domain(1, int, false);
    var a: [d] int;

    proc init(in d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete var field, concrete user initializer with const in intent");

  record R {
    var d: domain(1, int, false);
    var a: [d] int;

    proc init(const in d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic const field, generic user initializer with default intent");

  record R {
    const d;
    var a: [d] int;

    proc init(d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic const field, generic user initializer with in intent");

  record R {
    const d;
    var a: [d] int;

    proc init(in d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic const field, generic user initializer with const in intent");

  record R {
    const d;
    var a: [d] int;

    proc init(const in d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete const field, generic user initializer with default intent");

  record R {
    const d: domain(1, int, false);
    var a: [d] int;

    proc init(d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete const field, generic user initializer with in intent");

  record R {
    const d: domain(1, int, false);
    var a: [d] int;

    proc init(in d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete const field, generic user initializer with const in intent");

  record R {
    const d: domain(1, int, false);
    var a: [d] int;

    proc init(const in d) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic const field, concrete user initializer with default intent");

  record R {
    const d;
    var a: [d] int;

    proc init(d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic const field, concrete user initializer with in intent");

  record R {
    const d;
    var a: [d] int;

    proc init(in d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Generic const field, concrete user initializer with const in intent");

  record R {
    const d;
    var a: [d] int;

    proc init(const in d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete const field, concrete user initializer with default intent");

  record R {
    const d: domain(1, int, false);
    var a: [d] int;

    proc init(d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete const field, concrete user initializer with in intent");

  record R {
    const d: domain(1, int, false);
    var a: [d] int;

    proc init(in d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}

{
  writeHeader("Concrete const field, concrete user initializer with const in intent");

  record R {
    const d: domain(1, int, false);
    var a: [d] int;

    proc init(const in d: domain(1, int, false)) { this.d = d; }
  }

  testAll(R);
}
