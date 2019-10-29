class Grandparent {
    proc foo() {
        writeln(this.type :string);
    }
}

class Parent: Grandparent {
    override proc foo() {
        writeln(this.type :string);
        write("  ");
        super.foo();
    }
}

// foo calls its ancestor's foo, but Child does not override foo.
class Child: Parent {}

proc main() {
    var gp = new Grandparent();
    var p = new Parent();
    var c = new Child();

    gp.foo();
    p.foo();
    c.foo();
}
