use BlockDist, Reflection;

var Dist = new blockDist({1..10});
var Dom = {1..10} dmapped Dist;
var A: [Dom] real;

var SpsDom: sparse subdomain(Dom);
var SpsArr: [SpsDom] real;

var SpsDom2: sparse subdomain(A.domain);
var SpsArr2: [SpsDom2] real;

var B = blockDist.createArray({1..10}, int);
var SpsDom3: sparse subdomain(B.domain);
var SpsArr3: [SpsDom3] real;

writeln(A.domain.type:string);
writeln(A.domain.distribution.type:string);
writeln(isBlockDist(A.domain.distribution));
writeln();

writeln(SpsArr.domain.type:string);
writeln(SpsArr.domain.distribution.type:string);
writeln(isBlockDist(SpsArr.domain.distribution));
writeln();

writeln(SpsArr2.domain.type:string);
writeln(SpsArr2.domain.distribution.type:string);
writeln(isBlockDist(SpsArr2.domain.distribution));
writeln();

writeln(B.domain.type:string);
writeln(B.domain.distribution.type:string);
writeln(isBlockDist(B.domain.distribution));
writeln();

writeln(SpsArr3.domain.type:string);
writeln(SpsArr3.domain.distribution.type:string);
writeln(isBlockDist(SpsArr3.domain.distribution));
writeln();

proc isBlockDist(x: blockDist(?)) param {
  return true;
}

proc isBlockDist(x) param {
  return false;
}