proc mytuple return (1,2,3);
proc f(arg:int) return arg;

class NotDomainString {
  type t;
  param locking;
  var x = f(mytuple(1)); // calls 'this', 'f'
}

proc build_not_domain(type t) {
  var x = new unmanaged NotDomainString(t,true);
  return x;
}

var watDom = build_not_domain(string);
var x:borrowed CC;

class RR {
  var dom = build_not_domain(string);
}

class CC {
  proc this(idx: int):unmanaged RR { return new unmanaged RR(); }
}

proc f(arg:unmanaged CC):unmanaged RR return arg;
