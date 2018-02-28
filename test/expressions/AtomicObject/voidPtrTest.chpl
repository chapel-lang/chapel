class Obj { var x : int; var y : int; var z : int;}

proc fn(f : c_void_ptr) {
	writeln(f : Obj);
}


var obj : Obj = new Obj(1,2,3);
fn(obj : c_void_ptr);

