var i8:int(8);

i8 = 127;
writeln ("i(8)=", i8);

i8 = 128:int(8);
writeln ("i(8)=", i8);

i8 = -128;
writeln ("i(8)=", i8);

i8 = 256:int(8);
writeln ("i(8)=", i8);

i8 = (-256):int(8);
writeln ("i(8)=", i8);

var i32:int(32) = 2000000000;
writeln ("i(32)=", i32);
i8 = i32:int(8);
writeln ("i(8)=", i8);
