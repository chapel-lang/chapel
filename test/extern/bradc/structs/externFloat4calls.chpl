_extern record float4 {
  var x,y,z,w: real(32);
}

_extern def getfloat4(): float4;
_extern def printme(val: float4);

var myf42: float4 = getfloat4();
writeln("myf42 = ", myf42);
printme(myf42);
