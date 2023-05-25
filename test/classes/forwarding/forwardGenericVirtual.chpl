
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

var one = (new owned Rect(1)).borrow();
var three = (new owned Rect(3)).borrow();
var v = (new owned View(three)).borrow();

one.action(one);
v.action(three);
