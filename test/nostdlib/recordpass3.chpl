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

proc rfun(r:R) {
  return r.j;
}

proc main() {
  var r:R;
  r.j = 17;
  var x = rfun(r);
  var str = x:string;
  __primitive("chpl_error", str);
}

