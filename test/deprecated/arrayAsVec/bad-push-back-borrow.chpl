pragma "safe"
module BadPushBackBorrow {

class I {
  var dom: domain(1);
  var r: [dom] string; 
  proc init(in r: [?dom] string) {
    this.dom = dom;
    this.r = r;
  }
}

class D {
  var iArr: [1..0] borrowed I;
}

var i = new shared I(['a', 'b']);
var d = new owned D();

proc addone()
{
  // This should result in lifetime checking errror.
  //  * shared I destroyed at the end of this function
  //  * a borrow of it is saved in d.iArr.
  d.iArr.push_back(new shared I(i.r));
}

addone();

for element in d.iArr {
  writeln(element);
}

}
