record R {
  var a:int;
  var b:int;
  var c:int;
  var d:int;
  var e:int;
  var f:int;
  var g:int;
  var h:int;
  var i:int;
  var j:int;
  var k:int;
}

proc rfun(in x:R):R {
  x.j = 17;
  return x;
}

proc main() {
  var r:R;
  var r2:R;
  r2 = rfun(r);
  var num = r.j + r2.j;
  var str = num:string;
  __primitive("chpl_error", str);
}

