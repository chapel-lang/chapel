module M1 {
  var zzzSymbol = 1;
}
module M2 {
  var zzzSymbol = 2;
}

module MainMod {
  proc main {
    use M1, M2;
    var xyzSymbol = 1;
    __primitive(c"get visible symbols");
  }
}
