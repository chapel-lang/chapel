class Obj { var x : int; var y : int; var z : int;}
use CTypes;
proc fn(f : c_void_ptr) {
	writeln(f : borrowed Obj?);
}


var obj = (new owned Obj(1,2,3)).borrow();
fn(obj : c_void_ptr);

var obj2 : borrowed Obj = (new owned Obj(4,5,6)).borrow();
fn(obj2 : c_void_ptr);
