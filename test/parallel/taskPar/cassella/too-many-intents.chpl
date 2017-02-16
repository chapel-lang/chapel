var bruce: int;
sync begin with(ref bruce, const ref bruce, in bruce) { bruce = 7; }

writeln("now ", bruce);
