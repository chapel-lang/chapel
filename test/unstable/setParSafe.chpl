use Set;

// show that the 'parSafe' argument triggers an unstable warning on all 'set' initializers:

record Values {
    iter these() {
        for i in 0..10 do
            yield i;
    }
}

var v = new Values();

var s1 = new set(int, parSafe=true);
var s2 = new set(int, parSafe=false);
var s4 = new set(int, v, parSafe=true);
var s3 = new set(int, v, parSafe=false);
