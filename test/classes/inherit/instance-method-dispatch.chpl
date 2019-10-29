class Grandparent {
    proc foo() {
        writeln(this.type :string, ".foo");
    }
}

class Parent: Grandparent {
    override proc foo() {
        writeln(this.type :string, ".foo");
        write("  ");
        this.bar(); // foo calls bar.
    }

    proc bar() {
        writeln(this.type :string, ".bar");
    }
}

class Child: Parent {
    override proc bar() {
        writeln(this.type :string, ".bar");
    }
}

proc main() {
    var gp = new Grandparent();
    var p = new Parent();
    var c = new Child();

    gp.foo();
    p.foo();
    c.foo();
}
