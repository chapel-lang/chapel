config param low  = 2;
config param high = 4;

for i in 0..5 do
    if i in low..high then
        writeln("Yeah, ", i, "'s in ", low..high);
    else
        writeln("no way would ", i, " be in ", low..high, " :-/");

