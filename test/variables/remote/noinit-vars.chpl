record R {
  proc init() {
    writeln("R.init");
  }
}

writeln("yesinit");
on Locales[0] var A: [0..0] R;
writeln("noinit");
on Locales[0] var B: [0..0] R = noinit;

writeln("yesinit");
on Locales[0] var C, D: [0..0] R;
writeln("noinit");
on Locales[0] var E, F: [0..0] R = noinit;
