// this test matters because creating wrapper functions around contextManager
// calls does not work: they may or may not throw. This test makes sure the
// throwing forms still work.

record myManager : contextManager {
    var myResource: int;

    proc contextReturnType type do return int;

    proc enterContext() ref {
        writeln("entering context, resource = ", myResource);
        return myResource;
    }

    proc exitContext(in err: owned Error?) throws {
        writeln("exiting context, resource = ", myResource);
        if err {
            writeln("caught an error, will rethrow: ", err);
            throw err;
        }
    }
}

try {
    manage new myManager() as theResource {
        writeln("inside the manage statement");
        theResource = 42;
        throw new Error("I'm an error lol. lmao even");
        theResource = 1337;
    }
} catch err {
    writeln("caught error: ", err);
}
