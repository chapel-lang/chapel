record Foo {    
    proc Foo(s : string) {
        msg = "constructed with: " + s;
    }

    var msg = "default";
};

var f = new Foo("some message");
writeln(f.msg);

