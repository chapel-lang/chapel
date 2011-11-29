var a$ : sync bool;

begin {
    begin {
        begin {
        if a$ then writeln("impossible");
        }
        if a$ then writeln("impossible");
    }

    begin {
        if a$ then writeln("impossible");
    }
    if a$ then writeln("impossible");
}

begin {
    if a$ then writeln("impossible");
}


if a$ then writeln("impossible");
