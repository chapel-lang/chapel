use Python;


var code = """
def do_nothing():
    pass
def get_int():
    return 42
def pass_through(x):
    return x
""";

var interp = new Interpreter();
var mod = new Module(interp, '__empty__', code);
var do_nothing = new Function(mod, 'do_nothing');
var get_int = new Function(mod, 'get_int');
var pass_through = new Function(mod, 'pass_through');

{
  writeln("do_nothing()");

  var x = do_nothing();
  writeln(x, " - ", x.type:string);
  var y = do_nothing(NoneType);
  writeln(y, " - ", y.type:string);
  var z = do_nothing(owned Value);
  writeln(z, " - ", z.type:string);
  var w = do_nothing(owned Value?);
  writeln(w, " - ", w.type:string);

  var v: owned Value? = do_nothing();
  writeln(v, " - ", v.type:string);

  writeln("=====");
}

{
  writeln("get_int()");

  var x = get_int();
  writeln(x, " - ", x.type:string);
  var y = get_int(NoneType);
  writeln(y, " - ", y.type:string);
  var z = get_int(owned Value);
  writeln(z, " - ", z.type:string);
  var w = get_int(owned Value?);
  writeln(w, " - ", w.type:string);

  var v: owned Value? = get_int();
  writeln(v, " - ", v.type:string);

  writeln("=====");
}


{
  writeln("pass_through(17)");

  var x = pass_through(17);
  writeln(x, " - ", x.type:string);
  var y = pass_through(NoneType, 17);
  writeln(y, " - ", y.type:string);
  var z = pass_through(owned Value, 17);
  writeln(z, " - ", z.type:string);
  var w = pass_through(owned Value?, 17);
  writeln(w, " - ", w.type:string);

  var v: owned Value? = pass_through(17);
  writeln(v, " - ", v.type:string);

  writeln("=====");
}

{
  writeln("pass_through(None)");

  var x = pass_through(None);
  writeln(x, " - ", x.type:string);
  var y = pass_through(NoneType, None);
  writeln(y, " - ", y.type:string);
  var z = pass_through(owned Value, None);
  writeln(z, " - ", z.type:string);
  var w = pass_through(owned Value?, None);
  writeln(w, " - ", w.type:string);

  var v: owned Value? = pass_through(None);
  writeln(v, " - ", v.type:string);

  writeln("=====");
}


{
  writeln("pass_through(\"hi\")");

  var x = pass_through("hi");
  writeln(x, " - ", x.type:string);
  var y = pass_through(NoneType, "hi");
  writeln(y, " - ", y.type:string);
  var z = pass_through(owned Value, "hi");
  writeln(z, " - ", z.type:string);
  var w = pass_through(owned Value?, "hi");
  writeln(w, " - ", w.type:string);

  var v: owned Value? = pass_through("hi");
  writeln(v, " - ", v.type:string);

  writeln("=====");
}
