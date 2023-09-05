module FooExample {
    use IO, JSON;

    proc main() {
        class Foo {
           var name: string;
           var id: int;
           var isFoo: bool;
        }

        var mem = try! openMemFile();
        try! mem.writer().write('{"name":"fooOne", "id":1, "isFoo":false}');
        var reader = try! mem.reader(deserializer = new jsonDeserializer());
        var f = new Foo();
        try! reader.readf("%?", f);
        write("Foo: "); try! stdout.withSerializer(jsonSerializer).writeln(f);
    }
}
