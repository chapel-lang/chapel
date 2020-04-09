var bb = [{1..3, 1..3}, {1..5, 1..5}];
writeln(bb);
writeln(bb.type:string);
var b = bb: string;
writeln(b);
writeln(b.type: string); // [domain(1,int(64),false)] [domain(1,int(64),false)] string
