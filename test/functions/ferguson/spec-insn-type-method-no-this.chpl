// similar to spec-insn-method-no-this.chpl, but with type methods
record R {
  type t;
}

proc type (R(int)).foo() {
  type tmp = this.t;
  writeln("in specific R(int).foo, x=", tmp:string);
}

proc type (R(int)).bar() {
  type tmp = t; // as above, t refers to field, `this` is implicit
  writeln("in specific R(int).bar, x=", tmp:string);
}

proc getR type do return R(int);
proc type getR.baz() {
  type tmp = t; // as above, t refers to field, `this` is implicit
  writeln("in specific R(int).baz, x=", tmp:string);
}

type t = R(int);
t.foo();
t.bar();
t.baz();
