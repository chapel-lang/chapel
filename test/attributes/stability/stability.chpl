@unstable("this module is unstable")
module M {
  @unstable(category="draft", reason="the API is not finalized", issue="1233")
  class C {
    @deprecated("oldEnum is deprecated, use newEnum")
    enum oldEnum { a, b, c }

    enum newEnum { a=1, b=2, c=3 }

    @deprecated(since="1.29", notes="does not support new enum", suggestion="use methodtwo")
    proc methodone() { }
    // ignoring for now what should happen if an unstable class declares a
    // stable method, or if that even makes any sense
    @stable(since="1.29")
    proc methodtwo() { }
  }


}

module U {
  use M;
  proc main() {
    var c = new C();
    c.methodone(); // deprecated
  }
}
