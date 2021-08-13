record Foo {
    var x: int;
    var y: string;

    proc init(x: int, y: string){
        this.x = x + 1;
        this.y = y + "!";
    }

    proc init=(tup) where isTuple(tup){
        this.init(tup(0), tup(1));
    }

    operator :(tup, type t: Foo) {
        var f: Foo = tup;
        return f;
    }
}

var bar: Foo = new Foo(10, "Hi");
var baz: Foo = (11, "Bye");
const tup = (12, "Wait!");
var bab: Foo = tup;

writeln(bar);
writeln(baz);
writeln(bab);

writeln(isTuple(bar));
writeln(isTuple(baz));
writeln(isTuple(bab));
