proc check(param p) {
  param rel = __primitive("complex_get_real", p);
  param img = __primitive("complex_get_imag", p);
  writef("re(%.4n : %s) = %.20n : %s\n",
         p, p.type:string,
         rel, rel.type:string);
  writef("im(%.4n : %s) = %.20n : %s\n",
         p, p.type:string,
         img, img.type:string);
}

proc main() {
  writeln("complex");
  param c64: complex(64) = 3.0 - 4.0i;
  check(c64);
  param c128: complex(128) = 3.0 - 4.0i;
  check(c128);
}
