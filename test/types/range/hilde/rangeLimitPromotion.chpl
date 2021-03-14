// Expected: range(uint(64), bounded, false);
writeln((1:int(8)..100:uint(64)).type:string);

// Expected: range(uint(64), bounded, false);
writeln((1:int(16)..100:uint(64)).type:string);

// Expected: range(uint(64), bounded, false);
writeln((1:uint(64)..100:int(8)).type:string);
writeln((1:uint(64)..100:int(16)).type:string);
