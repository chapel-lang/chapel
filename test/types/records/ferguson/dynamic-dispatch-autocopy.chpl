class Parent {
  proc clone() : unmanaged Parent? {
    writeln("in Parent.clone()");
    return nil;
  }
}

class Child : Parent {
  override proc clone() : unmanaged Parent? {
    writeln("in Child.clone()");
    return new unmanaged Child();
  }
}



record R {
  var obj:unmanaged Parent? = nil;
  proc deinit() {
    delete obj;
  }
}

/* user record's can't write their own
   autoCopy after PR #5164
pragma "auto copy fn"
proc chpl__autoCopy(arg: R, definedConst: bool) {

  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  writeln("autoCopy");

  ret.obj = arg.obj.clone();

  return ret;
}
*/

// I'd like this to be ref, but that breaks
//    var outerX: R; begin { var x = outerX; }
pragma "init copy fn"
proc chpl__initCopy(arg: R, definedConst: bool) {
  // TODO - is no auto destroy necessary here?
  pragma "no auto destroy"
  var ret: R;

  writeln("initCopy");

  ret.obj = arg.obj!.clone();

  return ret;
}

proc doAutoCopyIt(x)
{
  return x;
}
proc main() {
  var r = new R(new unmanaged Child());
  writeln(doAutoCopyIt(r));
}

