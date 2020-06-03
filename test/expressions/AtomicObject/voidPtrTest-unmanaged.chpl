class Obj { var x : int; var y : int; var z : int;}

proc fn(f : c_void_ptr) {
	writeln(f : unmanaged Obj?);
}


var obj = new unmanaged Obj(1,2,3);
fn(obj : c_void_ptr);

var obj2 : unmanaged Obj = new unmanaged Obj(4,5,6);
fn(obj2 : c_void_ptr);

delete obj;
delete obj2;
