record RG {
  type t;
}

type bad = RG;
type good = RG(?);

record R {
  type badField = RG;
  type goodField = RG(?);
}

var x: R();
