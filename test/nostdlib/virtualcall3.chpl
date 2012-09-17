record R {
  var a:int(64);
  var b:int(64);
  var c:int(64);
  var d:int(64);
  var e:int(64);
  var f:int(64);
  var g:int(64);
}

class A {
  proc virt(x:R):R {
    var ret = x;
    ret.a = 1;
    return ret;
  }
}

class B : A {
  proc virt(x:R):R {
    var ret = x;
    ret.b = 1;
    return ret;
  }
}

proc main() {
  var b = new B();
  var r:R;
  r.c = 1;
  var got = b.virt(r);
  var str = got:string;
  __primitive("chpl_error", str);
}

