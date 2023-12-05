record RG {
  type t;
}

record R {
  type badField = RG;
  type goodField = RG(?);
}

var x: R();
