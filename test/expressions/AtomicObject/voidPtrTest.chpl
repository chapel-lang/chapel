class Obj { var x : int; var y : int; var z : int;}
use CTypes;
proc fn(f : c_ptr(void)) {
	writeln(f : borrowed Obj?);
}


var ownObj = new owned Obj(1,2,3);
var obj = ownObj.borrow();
fn(c_ptrTo(obj));

var ownObj2 = new owned Obj(4,5,6);
var obj2 : borrowed Obj = ownObj2.borrow();
fn(c_ptrTo(obj2));
