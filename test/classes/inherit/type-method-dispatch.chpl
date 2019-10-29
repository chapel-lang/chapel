class Grandparent {
    proc type foo() {
        writeln(this :string, ".foo");
    }
}

class Parent: Grandparent {
    /* override */ proc type foo() {
        writeln(this :string, ".foo");
        write("  ");
        this.bar(); // foo calls bar.
    }

    proc type bar() {
        writeln(this :string, ".bar");
    }
}

class Child: Parent {
    /* override */ proc type bar() {
        writeln(this :string, ".bar");
    }
}

proc main() {
    Grandparent.foo();
    Parent.foo();
    Child.foo();
}
