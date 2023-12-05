record RG {
  type t;
}

type bad = RG;
type good = RG(?);

class C {
  var x: int;
}

// not warning about generic management here
type genericMgmtC = C;
type ownedC = owned C;

class CG {
  type t;
}

type badC = CG;
type goodC = CG(?);

config param p = true;
config param notP = !p;

type badIfExpr1 = if p then RG else RG;
type badIfExpr2 = if notP then RG else RG;
type goodIfExpr1 = if p then RG(?) else RG(?);
type goodIfExpr2 = (if p then RG else RG)(?);
