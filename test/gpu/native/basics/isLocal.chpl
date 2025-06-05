use ChplConfig;

config const targetGpu = (CHPL_LOCALE_MODEL=="gpu");

record R {
  var x: int;
}

var x: int;
var r: R;

writeln(__primitive("is_local", x));
writeln(__primitive("is_local", r));
writeln(__primitive("is_local", r.x));

on if targetGpu then Locales.last.gpus[0] else Locales.last {
  writeln(__primitive("is_local", x));
  writeln(__primitive("is_local", r));
  writeln(__primitive("is_local", r.x));
}

