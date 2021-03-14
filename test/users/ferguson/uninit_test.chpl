
extern record mys {
  var a:int(32);
  var b:int(32);
}

record myr {
  var a:int(32);
  var b:int(32);
}

extern proc set_mys(ref x:mys);
extern proc ret_mys():mys;

proc testr():myr {
  var ret:myr;
  //set_mys(ret);
  //ret = ret_mys();
  ret.a = 0;
  ret.b = 1;
  return ret;
}

proc tests1():mys {
  var ret:mys;
  //set_mys(ret);
  //ret = ret_mys();
  ret.a = 1;
  ret.b = 2;
  return ret;
}

proc tests2():mys {
  var ret:mys;
  ret = ret_mys();
  return ret;
}

proc tests3():mys {
  var ret:mys;
  set_mys(ret);
  return ret;
}

writeln(testr());
writeln(tests1());
writeln(tests2());
writeln(tests3());

