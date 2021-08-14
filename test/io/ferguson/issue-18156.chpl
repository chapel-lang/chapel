module JsonStuff {
  use IO;
  use List;
  use Map;

  enum FooType { GOOD,BAD,UGLY }
  record Foo {
    var name: string;
    var id: int;
    var isFoo: bool;
    var fooType: FooType;
  }

  record Bar {
    var name: string;
    var id: int;
    var isBar: bool;
  }

  record FooBar {
    var name: string;
    var id: int;
    var isFooBar : bool;
    var foo: Foo;
    var bar: Bar;
    proc init(name: string, id: int, isFooBar : bool, foo: Foo, bar: Bar) {
      this.name=name;
      this.id=id;
      this.isFooBar = isFooBar;
      this.foo = foo;
      this.bar = bar;
    }
    proc init() {}
  }

  record FooBarGroup {
    var name: string;
    var id: int;
    var isFooBar : bool;
    var foos;
    var bars;
    proc init(name: string, id: int, isFooBar : bool, foos,  bars) {
      this.name=name;
      this.id=id;
      this.isFooBar = isFooBar;
      this.foos = foos;
      this.bars = bars;
    }
  }

  proc main() {
    var foo = new Foo('fooOne', 1, true, FooType.BAD);
    var fooTwo = new Foo('fooTwo', 2, false, FooType.GOOD);
    var bar = new Bar('barOne', 1, false);
    var barTwo = new Bar('barTwo', 2, true);
    var fb = new FooBar('fbOne', 2, false, foo, bar);
    var fbTwo = new FooBar('fbTwo', 2, false, foo, bar);
    try! writeln("Writing FooBar to JSON:  %jt".format(fb));
    var mem = try! openmem();
    var writer = try! mem.writer().write("%jt".format(fb));
    var reader = try! mem.reader();

    var f = new FooBar();
    try! reader.readf("%jt", f);

    try! writeln("Writing FooBar array to a JSON array %jt".format([fb,fbTwo]));
    try! writeln("Writing FooBar list to a JSON array %jt".format(new list([fb,fbTwo])));

    var fbg = new FooBarGroup('groupOne', 1, true, new list([foo,fooTwo]),new list([bar,barTwo]));
    var fbgJson = try! "%jt".format(fbg);
    try! writeln("Writing FooBarGroup to JSON: %jt".format(fbg));

    var fbm = new map(keyType=string,valType=FooBar);
    fbm.add('one', fb);
    fbm.add('two', fbTwo);

    try! writeln("Writing FooBar map to JSON: %jt".format(fbm));
  }
}
