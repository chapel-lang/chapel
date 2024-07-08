class C {
  type t;

  proc init(type t) {
    init this;
    this.t = t;
  }
}

var myC = new C(real);
