use BigInteger;

proc testOps(x, y:bigint) {
  writeln("Testing ops between ", x.type:string, " and bigint");

  writeln(x, " + ", y, " = ", x+y);
  writeln(y, " + ", x, " = ", y+x);

  writeln(x, " - ", y, " = ", x-y);
  writeln(y, " - ", x, " = ", y-x);

  writeln(x, " * ", y, " = ", x*y);
  writeln(y, " * ", x, " = ", y*x);

  //  writeln(x, " / ", y, " = ", x/y);
  writeln(y, " / ", x, " = ", y/x);;

  writeln(x, " == ", y, " = ", x==y);
  writeln(y, " == ", x, " = ", y==x);

  writeln(x, " != ", y, " = ", x!=y);
  writeln(y, " != ", x, " = ", y!=x);

  writeln(x, " < ", y, " = ", x<y);
  writeln(y, " < ", x, " = ", y<x);

  writeln(x, " <= ", y, " = ", x<=y);
  writeln(y, " <= ", x, " = ", y<=x);

  writeln(x, " > ", y, " = ", x>y);
  writeln(y, " > ", x, " = ", y>x);

  writeln(x, " >= ", y, " = ", x>=y);
  writeln(y, " >= ", x, " = ", y>=x);

  // with literal
  writeln(1:x.type, " + ", y, " = ", 1:x.type+y);
  writeln(y, " + ", 1:x.type, " = ", y+1:x.type);

  writeln(1:x.type, " - ", y, " = ", 1:x.type-y);
  writeln(y, " - ", 1:x.type, " = ", y-1:x.type);

  writeln(1:x.type, " * ", y, " = ", 1:x.type*y);
  writeln(y, " * ", 1:x.type, " = ", y*1:x.type);

  //  writeln(1:x.type, " / ", y, " = ", 1:x.type/y);
  writeln(y, " / ", 1:x.type, " = ", y/1:x.type);;

  writeln(1:x.type, " == ", y, " = ", 1:x.type==y);
  writeln(y, " == ", 1:x.type, " = ", y==1:x.type);

  writeln(1:x.type, " != ", y, " = ", 1:x.type!=y);
  writeln(y, " != ", 1:x.type, " = ", y!=1:x.type);

  writeln(1:x.type, " < ", y, " = ", 1:x.type<y);
  writeln(y, " < ", 1:x.type, " = ", y<1:x.type);

  writeln(1:x.type, " <= ", y, " = ", 1:x.type<=y);
  writeln(y, " <= ", 1:x.type, " = ", y<=1:x.type);

  writeln(1:x.type, " > ", y, " = ", 1:x.type>y);
  writeln(y, " > ", 1:x.type, " = ", y>1:x.type);

  writeln(1:x.type, " >= ", y, " = ", 1:x.type>=y);
  writeln(y, " >= ", 1:x.type, " = ", y>=1:x.type);

  // Casts to bigint
  writeln(x:bigint, " == ", new bigint(x), " = ", x:bigint == new bigint(x));
  writeln((x:bigint).type:string, " == ", (new bigint(x)).type:string, " = ", (x:bigint).type == (new bigint(x)).type);

  // String casts
  writeln((x:string):bigint, " == ", new bigint(x), " = ", (x:string):bigint == new bigint(x));
  writeln(new bigint(x:string), " == ", new bigint(x), " = ", new bigint(x:string) == new bigint(x));

  writeln();
}


const one = 1;
const two = 2;

const one_u = 1: uint;
const two_u = 2: uint;

const One = 1: bigint;
const Two = 2: bigint;

testOps(one, One);
testOps(one, Two);
testOps(one_u, One);
testOps(one_u, Two);
testOps(One, One);
testOps(One, Two);
