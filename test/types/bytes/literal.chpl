var b1 = b"engin";
writeln(b1, " as ", b1.type:string);

var b2 = b" ";
writeln(b2, " as ", b2.type:string);

var b3 = b"word1" + b" " + b"word2";
writeln(b3, " as ", b3.type:string);

proc funcWithConstRefArg(const ref b) {
  writeln(b, " as ", b.type:string);
}

funcWithConstRefArg(b"bytes literal");
