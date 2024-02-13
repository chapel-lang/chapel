class thing: contextManager {
    type T;
    var n: T;

    proc enterContext(): T {
        return this.n;
    }

    proc exitContext(in err: owned Error?) {
        if err then halt(err!.message());
    }
}

var x = new thing(int, 5);
