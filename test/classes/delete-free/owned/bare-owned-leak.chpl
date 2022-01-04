config param flag = true;

class C {
  var x: int;
  proc init(x: int) { writeln("C.init"); this.x = x; }
  proc deinit() { writeln("C.deinit"); }
}

new C(10);
new shared C(10);
new owned C(10);

writeln();

writeln("Begin anonymous scope");
{
  var a = new C(10);
  var b = new owned C(10);
  var c = new shared C(10);
  writeln(a);
  writeln(b);
  writeln(c);
}
writeln("End anonymous scope");
writeln();

writeln("Begin anonymous scope -- ifexpr");
{
  var a = if flag then new C(10) else 0;
  var b = if flag then new owned C(10) else 0;
  var c = if flag then new shared C(10) else 0;
  writeln(a);
  writeln(b);
  writeln(c);
}
writeln("End anonymous scope -- ifexpr");
writeln();

writeln("Begin: Record -- ifexpr in field declaration");
{
  record R {
    var field1 = if flag then new C(10) else 0;
    var field2 = if flag then new owned C(10) else 0;
    var field3 = if flag then new shared C(10) else 0;
  }
  writeln(new R());
}
writeln("End: Record -- ifexpr in field declaration");
writeln();

writeln("Begin: Record -- ifexpr in init");
{
  record R {
    var field1: C?;
    var field2: owned C?;
    var field3: shared C?;

    proc init() {
      field1 = if flag then new C(10) else 0;
      field2 = if flag then new owned C(10) else 0;
      field3 = if flag then new shared C(10) else 0;
    }
  }
  writeln(new R());
}
writeln("End: Record -- ifexpr in init");
writeln();

writeln("Begin: Function local scope");
{
  proc foo() {
    new C(10);
    new owned C(10);
    new shared C(10);
    return 1;
  }

  writeln(foo());
}
writeln("End: Function local scope");
writeln();

writeln("Begin: Function local scope -- local type");
{
  proc bar() {
    class D { }
    new D();
    new owned D();
    new shared D();
    return 2;
  }

  writeln(bar());
}
writeln("End: Function local scope -- local type");
writeln();

writeln("Begin: forall expr");
{
  var a = [i in 1..2] new C(10);
  var b = [i in 1..2] new owned C(10);
  var c = [i in 1..2] new shared C(10);

  writeln(a);
  writeln(b);
  writeln(c);
}
writeln("End: forall expr");
writeln();


