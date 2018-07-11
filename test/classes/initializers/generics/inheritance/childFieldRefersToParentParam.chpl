class BaseArr {
  param rank: int;

  proc init(param rank: int) {
    this.rank = rank;
  }
}

class ZeroBasedArr: BaseArr {
  var bounds: rank*int;

  /* This version correctly results in 'error: can't set value of field 'rank'
     from parent type during phase 1 of initialization:
  proc init(param rank: int) {
    this.rank = rank;
  }
  */

  // This version complains that we can't access param field 'rank'
  // during phase 1.  This access is due to the compiler's attempt to
  // get 'bounds' initialized prior to the call to super.init().
  proc init(param rank: int) {
    super.init(rank);
  }
}

var myArr = new unmanaged ZeroBasedArr(3);
writeln(myArr);
delete myArr;
