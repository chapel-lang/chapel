class C {
  var favorite: string;

  proc init() {
    favorite = "blue";
    favorite = "yellow";
  }
}

var c = new C();
writeln(c);
delete c;
