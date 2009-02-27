
var a$ : sync bool;

begin {
    writeln("In thread");
    if a$ then
        writeln("impossible");
}

if a$ then
    writeln("impossible");

