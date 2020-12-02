class Obj { var x : int; var y : int; var z : int;}
use CPtr;
proc fn(f : c_void_ptr) {
	writeln(f : borrowed Obj?);
}


var obj = new borrowed Obj(1,2,3);
fn(obj : c_void_ptr);

var obj2 : borrowed Obj = new borrowed Obj(4,5,6);
fn(obj2 : c_void_ptr);
