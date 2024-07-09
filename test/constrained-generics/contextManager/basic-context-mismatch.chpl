record myManager : contextManager {
    var myResource: int;

    proc contextReturnType type do return string;

    proc ref enterContext() ref {
        writeln("entering context, resource = ", myResource);
        return myResource;
    }

    proc exitContext(in err: owned Error?) {
        writeln("exiting context, resource = ", myResource);
    }
}

manage new myManager() as theResource {
    writeln("inside the manage statement");
    theResource = 42;
}
