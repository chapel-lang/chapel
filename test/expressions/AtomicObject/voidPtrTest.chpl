class Obj { var x : int; var y : int; var z : int;}

proc fn(f : c_void_ptr) {
	writeln(f : Obj);
}


var obj : Obj = new unmanaged Obj(1,2,3);
fn(obj : c_void_ptr);

var obj : Obj = new borrowed Obj(4,5,6);
fn(obj : c_void_ptr);
