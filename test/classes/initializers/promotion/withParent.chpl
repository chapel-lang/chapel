
pragma "use default init"
class Parent {
  type idxType;
}

pragma "use default init"
class HasGenericFields : Parent {
  param stridable : bool;

  proc chpl__promotionType() type return idxType;

  iter these() {
    var i : idxType;
    const r = if stridable then 1..100 by 10 else 1..10;
    for i in r do yield i;
  }
}

var h = new HasGenericFields(int, false);
writeln(h + 1);
delete h;

pragma "use default init"
class NoGenericFields : Parent {
  proc chpl__promotionType() type return idxType;

  iter these() {
    var i : idxType;
    for i in 1..10 do yield i;
  }
}

var n = new NoGenericFields(int);
writeln(n + 1);
delete n;
