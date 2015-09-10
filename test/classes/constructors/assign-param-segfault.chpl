enum Foo { Foo0, Foo1, Foo2, Foo3 };
enum Baz { Baz0, Baz1, Baz2, Baz3 };
class Bar {
param foo: Foo;
var baz: Baz;
proc Bar (param foo: Foo = Foo.Foo0) {
  //
  // This isn't legal, but shouldn't cause a segfault either
  //
  this.foo = foo;
    select foo {
      when Foo.Foo0 do this.baz = Baz.Baz0;
      when Foo.Foo1 do this.baz = Baz.Baz1;
      when Foo.Foo2 do this.baz = Baz.Baz2;
      when Foo.Foo3 do this.baz = Baz.Baz3;
      otherwise this.baz = Baz.Baz0;
        }
      }
}
var bar = new Bar(Foo.Foo1);
writeln(bar);
delete bar;
