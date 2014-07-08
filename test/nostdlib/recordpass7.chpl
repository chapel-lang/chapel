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

proc rfun(in x:R) {
  x.j = 17;
}

proc main() {
  var r:R;
  r.e = 2;
  rfun(r);
  var str = r:string;
  __primitive("chpl_error", str);
}

