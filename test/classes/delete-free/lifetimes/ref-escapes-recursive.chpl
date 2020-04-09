pragma "safe"
module refescapesrecursive {

proc ref_identity(ref x:int) ref return x;

proc bad1(i:int) ref :int {
  if i==0 {
    var x:int;
    return ref_identity(x);
  } else {
    return bad1(i-1);
  }
}

proc bad2(i:int) ref :int {
  var x:int;
  if i==0 {
    return ref_identity(x);
  } else {
    return bad2(i-1);
  }
}

proc bad3(i:int) ref :int {
  var x:int;
  if i==0 {
    ref tmp = ref_identity(x);
    return tmp;
  } else {
    return bad3(i-1);
  }
}


proc test() {
  ref a = bad1(1);
  writeln(a);
  ref b = bad2(2);
  writeln(b);
  ref c = bad3(3);
  writeln(c);
}

test();

}
