proc f(param a: int) param {
  return a;
}

proc g(param a: int) param : int {
  return a;
}

proc h(param a: int) : int {
  return a;
}

proc notAllControlFlowsHaveReturn(a:int):int {
  if (a>0) { return 0; }
}

writeln(f(3));
writeln(g(3));
writeln(h(3));
writeln();


proc g_rewritten(param a: int) param {
  param _ret = a;
  if (_ret.type != int) then
    compilerError("param function return type mismatch");
  return _ret;
}
writeln(g_rewritten(3));
writeln();
