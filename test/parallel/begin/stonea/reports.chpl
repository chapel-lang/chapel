
var a$ : sync bool;

begin {
    writeln("In thread");
    if a$.readFE() then
        writeln("impossible");
}

if a$.readFE() then
    writeln("impossible");

