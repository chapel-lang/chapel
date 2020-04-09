class C {
  const x, y: int;
  proc init(a: int) {
    this.x = make_x(a); // not OK, since it cannot access uninitialized memory
    this.y = make_y(a);

    inline proc make_x(a: int) const: int { return this.y + a + 1; }
    inline proc make_y(a: int) const: int { return this.x + a + 2; }
  }
}

var myC = new C(42);
writeln(myC);
