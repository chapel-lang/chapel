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

proc rfun(x:R):R {
  var ret = x;
  ret.j = 17;
  return ret;
}

proc main() {
  var r:R;
  r.e = 2;
  r = rfun(r);
  var str = r:string;
  __primitive("chpl_error", str);
}

