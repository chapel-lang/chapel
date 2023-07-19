
class Parent {
  type idxType;
}

class HasGenericFields : Parent {
  param stridable : bool;

  proc chpl__promotionType() type do return idxType;

  iter these() {
    var i : idxType;
    const r = if stridable then 1..100 by 10 else 1..10;
    for i in r do yield i;
  }
}

var ownH = new owned HasGenericFields(int, false);
var h = ownH.borrow();
writeln(h + 1);


class NoGenericFields : Parent {
  proc chpl__promotionType() type do return idxType;

  iter these() {
    var i : idxType;
    for i in 1..10 do yield i;
  }
}

var ownN = new owned NoGenericFields(int);
var n = ownN.borrow();
writeln(n + 1);
