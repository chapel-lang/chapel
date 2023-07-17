class Obj { var x : int; var y : int; var z : int;}
use CTypes;
proc fn(f : c_void_ptr) {
	writeln(f : borrowed Obj?);
}


var ownObj = new owned Obj(1,2,3);
var obj = ownObj.borrow();
fn(obj : c_void_ptr);

var ownObj2 = new owned Obj(4,5,6);
var obj2 : borrowed Obj = ownObj2.borrow();
fn(obj2 : c_void_ptr);
