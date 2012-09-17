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

proc rfun():R {
  var ret:R;
  ret.j = 17;
  return ret;
}

proc main() {
  var r:R = rfun();
  var str = r:string;
  __primitive("chpl_error", str);
}

