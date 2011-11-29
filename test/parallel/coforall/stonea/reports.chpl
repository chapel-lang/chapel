var a$ : sync bool;

begin {
    if a$ then writeln("impossible");
}

coforall i in 1..10 {
    if a$ then writeln("impossible");
}

begin {
    if a$ then writeln("impossible");
}

if a$ then writeln("impossible");
