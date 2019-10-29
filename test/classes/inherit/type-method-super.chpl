class Grandparent {
    proc type foo() {
        writeln(this :string);
    }
}

class Parent: Grandparent {
    /* override */ proc type foo() {
        writeln(this :string);
        write("  ");
        super.foo(); // equivalent to Grandparent.foo();
    }
}

// foo calls its ancestor's foo, but Child does not override foo.
class Child: Parent {}

proc main() {
    Grandparent.foo();
    Parent.foo();
    Child.foo();
}
