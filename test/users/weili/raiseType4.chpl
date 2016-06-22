proc raiseType(type x) where (x==int){
      var x:real;
      return x;
}

proc raiseType(type x) where (x==bool){
      var x:int;
      return x;
}
var h1:raiseType(int).type;
var h2:raiseType(bool).type;

writeln(h1.type:string);
writeln(h2.type:string);
