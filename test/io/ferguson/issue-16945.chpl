module FooExample {
    use IO;

    proc main() {
        class Foo {
           var name: string;
           var id: int;
           var isFoo: bool;
        }

        var mem = try! openmem();
        try! mem.writer().write('{"name":"fooOne", "id":1, "isFoo":false}');
        var reader = try! mem.reader();
        var f = new Foo();
        try! reader.readf("%jt", f);
        try! writeln("Foo: %jt".format(f));
    }
}

