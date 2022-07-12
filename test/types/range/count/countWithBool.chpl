config const flag = true;
config const lo = 0;

writeln(1..#false);
writeln(1..#true);
writeln(1..#flag);
writeln(lo..#flag);
writeln((lo..lo)#flag);
writeln((lo..lo)#false);
writeln((lo..lo)#true);
