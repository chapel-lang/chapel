use CyclicDist;

class VDistArray {
  const C: dmap(Cyclic(rank=1, idxType=int));
  var D: domain(1) dmapped C;
  var data: [D] int;
  proc init(val0: int, min: int, max: int) {
    this.C = new dmap(new Cyclic(min));
    this.D = {min..max};
  }
}

var foo = new unmanaged VDistArray(0, -4, 4);

for i in -4..4 do
  writeln(i, " ", foo.data[i].locale.id);

delete foo;
