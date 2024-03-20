var x = 42; 

writeln("before loop: ", x);
foreach i in 0..0 with (ref x) do x = 52;
writeln("after loop: ", x);
