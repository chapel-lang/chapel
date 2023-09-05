// this test matters because creating wrapper functions analogous to
// chpl__defaultHashWrapper around enterContext and exitContext, and
// calling them instead of invoking the methods directly, did not work:
// some enterContext calls throw, some don't. Thus, a chpl__enterContextWrapper
// would either not throw (making this test not work), or always throw, requiring
// catch statements for every manage statement.
//
// This test makes sure the throwing forms still work, while the other tests
// (implicitly) do the same for non-throwing forms. In this way, we ensure that
// whatever the implementation of context managers is, it doesn't suffer from
// the throw/no-throw defect of the wrapper function approach.

record myManager : contextManager {
    var myResource: int;

    proc contextReturnType type do return int;

    proc ref enterContext() ref {
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
