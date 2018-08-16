
class Base {
  // Removing this somehow avoids the bug...
  proc action(a) {
    halt("Should not see this!");
  }
}

class View : Base {
  var arr;

  forwarding arr;
}

class Rect : Base {
  param rank : int;

  proc inner(x) where x.rank == this.rank {
    writeln("Inner called on ", this.type:string, " with ", x.type:string);
  }

  proc action(a) {
    if a.rank != this.rank {
      compilerWarning("Rank mismatch between ", a.type:string, " and ", this.type:string);
    }
    inner(a);
  }
}

var one = new borrowed Rect(1);
var three = new borrowed Rect(3);
var v = new borrowed View(three);

one.action(one);
v.action(three);
