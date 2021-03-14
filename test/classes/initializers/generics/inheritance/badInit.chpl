class C {
  var name: string = "hi";
}

class D: C {
  type t;

  proc init(type t) {
    this.t = t;
  }

  proc init(s: string, type t) {
    this.name = s;
    this.t = t;
  }
}

var myD = new owned D("bye", int);
writeln(myD);
