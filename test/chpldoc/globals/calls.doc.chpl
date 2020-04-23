module Hello {

    module innerHello {
        proc writeGiven(x) {
            return x;
        }

        proc writeAdd(x, y) {
            return x+y;
        }
    }

    proc writeSomething() {
        return 5;
    }
}

pragma "no doc"
proc greet(name) {
    return "Hello " + name; 
}

// call with one argument int(64)
var variable : int(64) = 5;

// call with 1 argument
var hello0 : string = greet("Tony");

// sub call with no arguments
var hello1 : int = Hello.writeSomething();

// nested sub call with 1 argument
var hello2 : int = Hello.innerHello.writeGiven(6);

// nested sub call with 2 arguments
var hello3 : int = Hello.innerHello.writeAdd(7, 8);
