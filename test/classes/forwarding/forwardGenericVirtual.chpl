
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

var oneObj = new Rect(1);      var one = oneObj.borrow();
var threeObj = new Rect(3);    var three = threeObj.borrow();
var viewObj = new View(three); var v = viewObj.borrow();

one.action(one);
v.action(three);
