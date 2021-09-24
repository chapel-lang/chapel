// Checks basic deprecated declarations of modules
deprecated module A {
  deprecated module InnerA { }
}

deprecated "B is deprecated, use C instead" module B {
  deprecated "InnerB is deprecated, use C.InnerC2 instead" module InnerB { }
}

module C {
  // Checks deprecated nested module in undeprecated outer module
  deprecated module InnerC { }
  module InnerC2 { }
}

// Checks interaction with documentation
/* There was documentation of this symbol */
deprecated module D {
  /* There was documentation of this symbol */
  deprecated module InnerD { }
}

/* This symbol also was documented */
deprecated "E is deprecated, use F instead" module E {
  /* This symbol also was documented */
  deprecated "InnerE is deprecated, use F.InnerF2 instead" module InnerE { }
}

module F {
  // Checks deprecated nested module in undeprecated outer module
  /* This symbol also was documented */
  deprecated "InnerF is deprecated, use InnerF2 instead" module InnerF { }
  module InnerF2 { }
}

// Checks interaction when documentation mentions deprecation in some form
/* This symbol is deprecated */
deprecated module G {
  /* This symbol is deprecated */
  deprecated module InnerG { }
}

/* This symbol is also deprecated, please use I instead */
deprecated "H is deprecated, use I instead" module H {
  /* This symbol is also deprecated, please use I.InnerI2 instead */
  deprecated "InnerH is deprecated, use I.InnerI2 instead" module InnerH { }
}

/* Replaces the deprecated H */
module I {
  /* This symbol is also deprecated, please use I.InnerI2 instead */
  deprecated module InnerI { }
  module InnerI2 { }
}
// Ensures deprecation doesn't cause "no doc" symbols to turn up in
// documentation
pragma "no doc"
deprecated module J {
  pragma "no doc"
  deprecated module InnerJ { }
}

pragma "no doc"
deprecated "K is deprecated, use L instead" module K {
  pragma "no doc"
  deprecated "InnerK is deprecated, use L.InnerL2 instead" module InnerK { }
}

module L {
  // Checks deprecated nested module in undeprecated outer module
  pragma "no doc"
  deprecated module InnerL { }
  module InnerL2 { }
}
