/*********************************************************/
/*		ParaCR algorithm in Chapel               */		
/*		    Block distribution                   */
/*	Loop unrolled to avoid a deep copy of arrays     */
/* Contributed by Juan Lopez, Alberto Sanz & Rafa Asenjo */
/*********************************************************/

//This code solves a dense tridiagonal system following the PARACR algorithm

use Time;
use BlockDist;

//Problem Dimension
config const n:int=16;

var stages:int= GetNumStages(n);
//writeln("Stages->",stages);

var error:int=0;

const Space = {1..n};
const Dom = Space dmapped Block(boundingBox=Space);

var Dstages: domain(1,int)={1..stages};
var AA,BB,CC,DD:[Dom] real;
var A,B,C,D,X:[Dom] real;

config const timer : bool =false;

//Timing variables
var t1,t2,t3,t4 :real; 
var CalcTime:[Dstages] real=0.0;
var TotCalcTime: real=0.0;

proc main(){

/* Algoritmo de reduccion ciclica paralela */
  SetExampleMatrix();
  
  if(n==1) {
    X(1)=D(1)/B(1);
    writeln("Done!!. Too easy... bye");
    exit(0);
  }

  if timer then t1=getCurrentTime();

/*********************/
/* Elimination Phase */
/*********************/
  //Unrolled loop: it does two iterations (stages) in the body
  for j in 1..stages-1 by 2 do { 
//  Odd stage (j)
    ComputeStage(AA,BB,CC,DD,A,B,C,D,j);
//  Even stage (j+1)
    ComputeStage(A,B,C,D,AA,BB,CC,DD,j+1);
  }

  if(stages&1){ //If odd number of stages there is one more odd stage missing
    ComputeStage(AA,BB,CC,DD,A,B,C,D,stages);
  }

/**********************/
/* Substitution Phase */
/**********************/
	
  if(stages&1){ //If odd number of stages, diagonal is in BB and RHS in DD
    forall (x,d,b) in zip(X,DD,BB) do x=d/b;
  }
  else{ //else, diagonal is in B and RHS in D
    forall (x,d,b) in zip(X,D,B) do x=d/b;
  }

  if timer then t2=getCurrentTime();
	
  //writeln("Tridiagonal System Solution:");
  //PrintV(X);
  SetExampleMatrix();
  //PrintV(D);
  Check();	 
   
  if(error!=1)
    {
      writeln("Done!");
      if timer then {
	//writeln("Stages breakdown time (s) --> ", CalcTime);
	//writeln("Time from Block D. to Cyclic D.: ",t6-t5);
	//writeln("Stages computing time --> ", TotCalcTime,"sec.");
	writeln("Time elapsed : ", t2-t1," seconds" );
      }
    }
  else
    writeln("WRONG!!");
} //End Main

proc ComputeStage(A,B,C,D,AA,BB,CC,DD,j, msg="")
{
  //  writeln("ComputeStage", msg, " j=", j);
  // if timer then t3=getCurrentTime();
  const TtS:int=1<<(j-1); //// TtS stand for "Two to the Stage (minus 1)" which is = 2**(j-1)
  forall i in Dom do{
    //writeln("i ",i, " j ",j);
    const lo=i-TtS;
    const hi=i+TtS;
    if lo<1 then{
      if hi>n then{ // Case 1: out of lower and upper range
	A(i)=0;
	  B(i)=BB(i);
	  C(i)=0;
	  D(i)=DD(i);
      } else { // Case 2: out of lower range
	const c=CC(i),bh=BB(hi);		
	A(i)=0;
	B(i)=(BB(i) - (c*AA(hi)/bh));
	C(i)=(-c*CC(hi))/bh;
	D(i)=DD(i) - (DD(hi)*c)/bh;
      }
    }else {
      if hi>n then{ // Case 3: out of upper range
	const a=AA(i),bl=BB(lo);	
	A(i)=(-AA(lo)*a)/bl;
	B(i)=(BB(i) - (CC(lo)*a/bl));
	C(i)=0;
	D(i)=DD(i) - (a*DD(lo))/bl;
      } else { // Case 4: no equation out of range
	const a=AA(i),bh=BB(hi),bl=BB(lo),c=CC(i);
	A(i)=(-AA(lo)*a)/bl;
	B(i)=(BB(i) - (CC(lo)*a/bl) - (c*AA(hi)/bh));
	C(i)=(-c*CC(hi))/bh;
	D(i)=DD(i) - (a*DD(lo))/bl - (DD(hi)*c)/bh;
      }//if
    }//if
  }//for all i
  //if timer then {
  //  t4=getCurrentTime();
  //  CalcTime[j]=t4-t3;
  //  TotCalcTime+=CalcTime[j];
  //}
}

proc Check()
{
  var ERROR: real = 0.00001;
  forall i in Dom with (ref error)
    {
      if(i==1){
	if (abs(B[i]*X[i] + C[i]*X[i+1] - D[i]) > ERROR)
	  { writeln("Case 1 failed"); error = 1;}}
      else if(i==n){
	if (abs(A[i]*X[i-1] + B[i]*X[i] - D[i]) > ERROR)
	  { writeln("Case 2 failed"); error = 1;}}
      else{
	if(abs(A[i]*X[i-1] + B[i]*X[i] + C[i]*X[i+1]-D[i])>ERROR)
	  { writeln("Caso 3 failed"); error = 1;}}
    }
}

proc PrintV(X)
{
  for i in Dom do{
    writeln("V[",i,"] = ",X(i));
  }
}

proc SetExampleMatrix()
{
  forall (i) in Dom{
    A(i)=1.0;
    B(i)=2.0;
    C(i)=1.0;
  }
  
  A(1)=0;C(n)=0;
  forall i in 1..(n+1)/2 do {
    D(i)=i;
    D(n-i+1)=i;
  }

  //writeln("Example matrix generated");
  //  writeln("D=",D);

}

proc GetNumStages(n: int)
{
  if n-2**log2(n) != 0 then
    return (log2(n)+1);
  else
    return log2(n);
}
