enum foo {@unstable a, b, c};
enum foo2 {@unstable a,};

proc main() {
    var trig = foo.a;   //triggered here because a in foo is what's marked
    var trig2 = foo2.a; //triggered here

    writeln(trig);  //not triggered here because unstable isn't attached to trig or trig2
    writeln(trig2); //not triggered here

    writeln(foo.a); //would be triggered because a is what's marked as unstable
}

main();