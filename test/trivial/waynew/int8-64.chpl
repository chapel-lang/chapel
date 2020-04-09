proc jam_records( somei:int(64)) : void {
  writeln( somei);
}

var i:int;
i = 510;
writeln ("i=510 is ", i);

var i8:int(8);
i8 = 127;
writeln ("i(8)=127 is ", i8);
i8 = 128:int(8);
writeln ("i(8)=128 is ", i8);
i8 = -128;
writeln ("i(8)=-128 is ", i8);
i8 = 256:int(8);
writeln ("i(8)=256 is ", i8);
i8 = (-256):int(8);
writeln ("i(8)=-256 is ", i8);

var i16:int(8+8);
i16 = 1024;
writeln ("i(16)=1024 is ", i16);
i16 = 16*1024;
writeln ("i(16)=16*1024 is ", i16);
i16 = (32*1024):int(16);
writeln ("i(16)=32*1024 is ", i16);
i16 = (64*1024):int(16);
writeln ("i(16)=64*1024 is ", i16);

var i32:int(32) = 2000000000;
writeln ("i(32)=2000000000 is ", i32);
i8 = i32:int(8);
writeln ("i(8)=i(32) is ", i8);

var i64:int(64) = 400000000000;
writeln ("i(64)=400000000000 is ", i64);

jam_records( i);
jam_records( i8);
jam_records( i16);
jam_records( i32);
jam_records( i64);


