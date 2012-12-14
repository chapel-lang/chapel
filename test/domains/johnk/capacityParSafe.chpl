var D : domain(string);

coforall i in 1..1000 {
    D.requestCapacity(i);
}

writeln("SUCCESS");
