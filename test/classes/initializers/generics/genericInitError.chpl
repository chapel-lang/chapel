class C {
  type t;

  proc init(type t) {
    this.complete();
    this.t = t;
  }
}

var myC = new C(real);
