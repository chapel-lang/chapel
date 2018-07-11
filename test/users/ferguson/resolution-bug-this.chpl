var watDom: domain(string);
var wat: [watDom] WAT;

class WAT { }

class R {
  var dom: domain(string); // <-- unable to resolve this type
}

class C {
  proc this(idx: int):R { return new R(); }
}
