var watDom: domain(string, parSafe=false);
var wat: [watDom] unmanaged WAT?;

class WAT { }

class R {
  var dom: domain(string, parSafe=false); // <-- unable to resolve this type
}

class C {
  proc this(idx: int):unmanaged R { return new unmanaged R(); }
}
