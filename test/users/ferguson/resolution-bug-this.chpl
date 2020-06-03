var watDom: domain(string);
var wat: [watDom] unmanaged WAT?;

class WAT { }

class R {
  var dom: domain(string); // <-- unable to resolve this type
}

class C {
  proc this(idx: int):unmanaged R { return new unmanaged R(); }
}
