// You can compile this code and run it without problem
//But if you compeile with chpl -sdebugDefaultDistBulkTransfer then:
/*
chpl -sdebugDefaultDistBulkTransfer rafatest2.chpl && ./a.out -nl 2
A is:1 2 3 4 5 6 7 8 9 10
B before copy:2 4 6 8 10 12 14 16 18 20
	local get() from 0
*** Caught a fatal signal: SIGSEGV(11) on node 1/2
 */
//The problem is in this piece of code in DefaultRectangular.chpl:
/*  if this.data.locale.id==here.id {
      if debugDefaultDistBulkTransfer then
         writeln("\tlocal get() from ", B.locale.id); */
//But if you comment line 32 and uncomment line 31 works like a charm (????) WHY?

use BlockDist;

config const n=10;
const S = [1..n];
const BDist = new dmap(new Block(boundingBox=S));
const BDom : domain(1) dmapped BDist=S;

//var A : [[1..n] dmapped Block(rank=1,boundingBox=[1..n])] int;
var A : [BDom] int;

for (i) in A.domain {
       A(i)=i;
}
writeln("A is:",A);

//var B : [[1..n] dmapped Block(rank=1,boundingBox=[1..n])] int; //uncomment this
var B : [BDom] int;                                              //and comment this
for (i) in B.domain {
       B(i)=2*i;
}
writeln("B before copy:",B);
B=A;

writeln("B after B=A:",B);