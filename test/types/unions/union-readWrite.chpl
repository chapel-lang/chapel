
use IO;
use Reflection;

union U {
  var x : int;
  var y : real;
  var z : complex(128);
}

proc helper(param name : string) {
  writeln("---------------");
  var f = openmem();
  var x : U;

  __primitive("set_union_id", x, getFieldIndex(U, name) + 1);
  ref field = getFieldRef(x, name);
  if field.type == int then field = 5;
  else if field.type == real then field = 42.0;
  else field = 12.0 + 34i;

  {
    writeln("writing: ", x);
    var w = f.writer();
    w.write(x);
  }

  {
    var s : string;
    var r = f.reader();
    r.readAll(s);
    writeln("file contents: ", s);
  }

  {
    var y : U;
    var r = f.reader();
    r.read(y);
    writeln("read: ", y);
  }
}

proc main() {
  helper("x");
  helper("y");
  helper("z");
}
