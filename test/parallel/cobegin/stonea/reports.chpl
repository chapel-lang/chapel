use Time;

var a$ : sync bool;

cobegin {
    {sleep(1); if a$ then writeln("impossible"); }
    {sleep(1); if a$ then writeln("impossible"); }
    {sleep(1); if a$ then writeln("impossible"); }
    {sleep(1); if a$ then writeln("impossible"); }
}

if a$ then writeln("impossible");
