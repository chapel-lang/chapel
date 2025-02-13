
use Python;

var code = """
class C:
  def __init__(self):
    self.x = 17
  def foo(self):
    return self.x

def bar():
  return 18
""";

var interp = new Interpreter();
var mod = interp.importModule('__empty__', code);

{
  var res1 = mod.call('bar');

  var bar = mod.get('bar');
  var res2 = bar();

  var res3 = mod.get('bar')();

  // these are all the same
  writeln("mod.call('bar'): ", res1);
  writeln("bar(): ", res2);
  writeln("mod.get('bar')(): ", res3);
}

{
  var C = mod.get('C');
  var c = C();
  var res1 = c.call('foo');

  var foo = c.get('foo');
  var res2 = foo();

  var res3 = mod.get('C')().call('foo');

  // these are all the same
  writeln("c.call('foo'): ", res1);
  writeln("foo(): ", res2);
  writeln("mod.get('C')().call('foo'): ", res3);
}
