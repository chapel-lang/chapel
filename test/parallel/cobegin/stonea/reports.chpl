use Time;

var a$ : sync bool;

cobegin {
    {sleep(1); if a$.readFE() then writeln("impossible"); }
    {sleep(1); if a$.readFE() then writeln("impossible"); }
    {sleep(1); if a$.readFE() then writeln("impossible"); }
    {sleep(1); if a$.readFE() then writeln("impossible"); }
}

if a$.readFE() then writeln("impossible");
