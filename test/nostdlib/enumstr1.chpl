enum F { a=1, b, c, d=-3, e, f };

proc tostring(x:F) {
  if x == -3 then return "OK";
  else return "BAD";
}

proc main() {
  var str = tostring(F.d);
  __primitive("chpl_error", str);
}

