use driver_domains;

var x1 = Dom1D;
var x2 = Dom2D;
var x3 = Dom3D;
var x4 = Dom4D;
var x5 = Dom2D32;

writeln(x1.type:string == Dom1D.type:string);
writeln(x2.type:string == Dom2D.type:string);
writeln(x3.type:string == Dom3D.type:string);
writeln(x4.type:string == Dom4D.type:string);
writeln(x5.type:string == Dom2D32.type:string);

writeln(x1.dims() == Dom1D.dims());
writeln(x2.dims() == Dom2D.dims());
writeln(x3.dims() == Dom3D.dims());
writeln(x4.dims() == Dom4D.dims());
writeln(x5.dims() == Dom2D32.dims());
