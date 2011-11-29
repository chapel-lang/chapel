
extern class mys {
    var x:int(32);
}

extern proc return_mys(inout x:mys);

var x:mys = new mys(6);
var y:mys = x;

// sets x->x to 7, x to nil
return_mys(x);

writeln(x);
writeln(y);

