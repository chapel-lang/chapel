record R {
  
  var x;
}

proc foo(type t:R) {
  writeln("generic ", t:string);
}
proc foo(type t:R(real)) {
  writeln("real ", t:string);
}


var r1=new R(1);
var rs=new R("test");
var rr=new R(1.0);

foo(r1.type);
foo(rs.type);
foo(rr.type);
