class C {
  type t;

  proc init(type t) {
    this.initDone();
    this.t = t;
  }
}

var myC = new C(real);
