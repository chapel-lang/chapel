var a$ : sync bool;

begin {
    begin {
        begin {
        if a$.readFE() then writeln("impossible");
        }
        if a$.readFE() then writeln("impossible");
    }

    begin {
        if a$.readFE() then writeln("impossible");
    }
    if a$.readFE() then writeln("impossible");
}

begin {
    if a$.readFE() then writeln("impossible");
}


if a$.readFE() then writeln("impossible");
