proc notParam(x) return x;
proc f(param p = notParam(1)) { }

f();
