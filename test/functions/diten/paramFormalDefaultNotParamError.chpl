proc notParam(x) do return x;
proc f(param p = notParam(1)) { }

f();
