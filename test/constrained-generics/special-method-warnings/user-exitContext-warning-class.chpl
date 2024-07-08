class C {
    var x: uint;

    proc exitContext(in err: owned Error?) {}
}

writeln(new C());
