use BlockDist;
use PrintComms;
config const printOutput=false;
config const doDiagnostics=false;

config  const n: int=8;
var e=false;
var Dist1 = new dmap(new Block({1..n,1..n}));
var Dist2 = new dmap(new Block({1..(2*n),1..(2*n)}));
var Dom1: domain(2,int) dmapped Dist1 = {1..n,1..n};
var Dom2: domain(2,int) dmapped Dist2 = {1..2*n,1..2*n};

proc main(){

  var A1:[Dom1] int(64); 
  var B1:[Dom2] int(64);
  var a,b:real;
  var i:int(64);
  for (a,i) in zip(A1,{1..n*n}) do a=i;
  for (b,i) in zip(B1,{1..2*2*n*n}) do b=i+100;

  var DomA = {1..3,2..7};
  var DomB = {3..5,5..10};

  A1[DomA]=B1[DomB];

  if printOutput {
    writeln("Example 1) int(64) A",DomA," <-- B",DomB);
    writeln("A= ",A1);
    writeln("B= ",B1);
  }
  for (a,b) in zip(A1[DomA],B1[DomB]) do if (a!=b) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  //EXAMPLE 2
  var A2:[Dom1] int(32); 
  var B2:[Dom2] int(32);
  var a2,b2:int(32);
  for (a2,i) in zip(A2,{1..n*n}) do a2=i:int(32);
  for (b2,i) in zip(B2,{1..2*2*n*n}) do b2=i:int(32)+100;
  //A2=1:int(32)..;
  //B2=100:int(32)..;
  
  A2[DomA]=B2[DomB];
  
  if printOutput {
    writeln("Example 2) int(32) A",DomA," <-- B",DomB);
    writeln("A= ",A2);
    writeln("B= ",B2);
  }
  for (a2,b2) in zip(A2[DomA],B2[DomB]) do if (a2!=b2) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 3
  var A3:[Dom1] int(16); 
  var B3:[Dom2] int(16);
  var a3,b3:int(16);
  for (a3,i) in zip(A3,{1..n*n}) do a3=i:int(16);
  for (b3,i) in zip(B3,{1..2*2*n*n}) do b3=i:int(16)+100;
  
  A3[DomA]=B3[DomB];
  
  if printOutput {
    writeln("Example 3) int(16) A",DomA," <-- B",DomB);
    writeln("A= ",A3);
    writeln("B= ",B3);
  }
  for (a3,b3) in zip(A3[DomA],B3[DomB]) do if (a3!=b3) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 4
  var A4:[Dom1] int(8); 
  var B4:[Dom2] int(8);
  var a4,b4:int(8);
  for (a4,i) in zip(A4,{1..n*n}) do a4=i:int(8);
  for (b4,i) in zip(B4,{1..2*2*n*n}) do b4=i:int(8)+100;
  
  A4[DomA]=B4[DomB];
  
  if printOutput {
    writeln("Example 4) int(8) A",DomA," <-- B",DomB);
    writeln("A= ",A4);
    writeln("B= ",B4);
  }
  for (A4,B4) in zip(A4[DomA],B4[DomB]) do if (a4!=b4) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  //EXAMPLE 5
  var A5:[Dom1] real(64); 
  var B5:[Dom2] real(64);
  var a5,b5:real(64);
  for (a5,i) in zip(A5,{1..n*n}) do a5=i:real(64);
  for (b5,i) in zip(B5,{1..2*2*n*n}) do b5=(i+100):real(64);
  
  A5[DomA]=B5[DomB];
  
  if printOutput {
    writeln("Example 5) real(64) A",DomA," <-- B",DomB);
    writeln("A= ",A5);
    writeln("B= ",B5);
  }
  for (a5,b5) in zip(A5[DomA],B5[DomB]) do if (a5!=b5) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 6
  var A6:[Dom1] real(32); 
  var B6:[Dom2] real(32);
  var a6,b6:int;
  
  for (a6,i) in zip(A6,{1..n*n}) do a6=i:real(32);
  for (b6,i) in zip(B6,{1..2*2*n*n}) do b6=(i+100):real(32);
  
  A6[DomA]=B6[DomB];
  
  if printOutput {
    writeln("Example 6) real(32) A",DomA," <-- B",DomB);
    writeln("A= ",A6);
    writeln("B= ",B6);
  }
  for (a6,b6) in zip(A6[DomA],B6[DomB]) do if (a6!=b6) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  //EXAMPLE 7
  var A7:[Dom1] uint; 
  var B7:[Dom2] uint;
  var a7,b7:uint;
  for (a7,i) in zip(A7,{1..n*n}) do a7=i:uint;
  for (b7,i) in zip(B7,{1..2*2*n*n}) do b7=(i+100):uint;
  
  A7[DomA]=B7[DomB];
  
  if printOutput {
    writeln("Example 7) uint(64) A",DomA," <-- B",DomB);
    writeln("A= ",A7);
    writeln("B= ",B7);
  }
  for (a7,b7) in zip(A7[DomA],B7[DomB]) do if (a7!=b7) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 8
  var A8:[Dom1] uint(32); 
  var B8:[Dom2] uint(32);
  var a8,b8:uint(32);
  for (a8,i) in zip(A8,{1..n*n}) do a8=i:uint(32);
  for (b8,i) in zip(B8,{1..2*2*n*n}) do b8=(i+100):uint(32);
  
  A8[DomA]=B8[DomB];
  
  if printOutput {
    writeln("Example 8) uint(32) A",DomA," <-- B",DomB);
    writeln("A= ",A8);
    writeln("B= ",B8);
  }
  for (a8,b8) in zip(A8[DomA],B8[DomB]) do if (a8!=b8) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 9
  var A9:[Dom1] uint(16); 
  var B9:[Dom2] uint(16);
  var a9,b9:uint(16);
  for (a9,i) in zip(A9,{1..n*n}) do a9=i:uint(16);
  for (b9,i) in zip(B9,{1..2*2*n*n}) do b9=(i+100):uint(16);
  
  A9[DomA]=B9[DomB];
  
  if printOutput {
    writeln("Example 9) uint(16) A",DomA," <-- B",DomB);
    writeln("A= ",A9);
    writeln("B= ",B9);
  }
  for (a9,b9) in zip(A9[DomA],B9[DomB]) do if (a9!=b9) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 10
  var A10:[Dom1] uint(8); 
  var B10:[Dom2] uint(8);
  var a10,b10:uint(8);
  for (a10,i) in zip(A10,{1..n*n}) do a10=i:uint(8);
  for (b10,i) in zip(B10,{1..2*2*n*n}) do b10=(i+100):uint(8);
  
  A10[DomA]=B10[DomB];
  
  if printOutput {
    writeln("Example 10) uint(8) A",DomA," <-- B",DomB);
    writeln("A= ",A10);
    writeln("B= ",B10);
  }
  for (a10,b10) in zip(A10[DomA],B10[DomB]) do if (a10!=b10) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
  
  //EXAMPLE 11
  var h:int;
  var A11:[Dom1] complex; 
  var B11:[Dom2] complex;
  var a11,b11:complex;
  for (a11,h) in zip(A11,{1..n*n}) do a11=(h+1i):complex;
  for (b11,h) in zip(B11,{1..2*2*n*n}) do b11=(h+100+2i):complex;
  
  A11[DomA]=B11[DomB];
  
  if printOutput {
    writeln("Example 11) complex(128) A",DomA," <-- B",DomB);
    writeln("A= ",A11);
    writeln("B= ",B11);
  }
  for (a11,b11) in zip(A11[DomA],B11[DomB]) do if (a11!=b11) then {writeln("ERROR!!!!: ");e=true;};
  if printOutput {
    writeln();
  }

  //EXAMPLE 12
  var A12:[Dom1] complex(64); 
  var B12:[Dom2] complex(64);
  var a12,b12:complex(64);
  for (a12,h) in zip(A12,{1..n*n}) do a12=(h+3i):complex(64);
  for (b12,h) in zip(B12,{1..2*2*n*n}) do b12=(h+100 + 4i):complex(64);
  var DomAA = {1..3,2..7};
  var DomBB = {3..7 by 2,5..10};
  A12[DomAA]=B12[DomBB];
  
  if printOutput {
    writeln("Example 12) complex(64) A",DomAA," <-- B",DomBB);
    writeln("A= ",A12);
    writeln("B= ",B12);
  }
  for (a12,b12) in zip(A12[DomAA],B12[DomBB]) do if (a12!=b12) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  //EXAMPLE 13
  var A13:[Dom1] imag; 
  var B13:[Dom2] imag;
  var a13,b13:imag;
  for (a13,h) in zip(A13,{1..n*n}) do a13=(h*1.0i):imag;
  for (b13,h) in zip(B13,{1..2*2*n*n}) do b13=(100+h*1.0i):imag;
  
  A13[DomA]=B13[DomB];
  
  if printOutput {
    writeln("Example 13) imag(64) A",DomA," <-- B",DomB);
    writeln("A= ",A13);
    writeln("B= ",B13);
  }
  for (a13,b13) in zip(A13[DomA],B13[DomB]) do if (a13!=b13) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
   
  //EXAMPLE 14
  var A14:[Dom1] imag(32); 
  var B14:[Dom2] imag(32);
  var h2:int;
  var a14,b14:imag(32);
  for (a14,h2) in zip(A14,{1..n*n}) do a14=(h2*1.0i):imag(32);
  for (b14,h2) in zip(B14,{1..2*2*n*n}) do b14=(100+h2*1.0i):imag(32);
  
  A14[DomA]=B14[DomB];
  
  if printOutput {
    writeln("Example 14) imag(32) A",DomA," <-- B",DomB);
    writeln("A= ",A14);
    writeln("B= ",B14);
  }
  for (a14,b14) in zip(A14[DomA],B14[DomB]) do if (a14!=b14) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }

  //EXAMPLE 15
  class C{
    var a:int;
    var b:real;
    proc print(){writeln("a:",a," b:",b);}
  }
  var p1 = new C();
  var p2 = new C();
  var c1:[Dom1] C.type;
  var c2:[Dom2] C.type;
  
  for p in zip(c1) do p= new C(1,2.0);
  for p in zip(c2) do p= new C(3,4.0);

  c1[DomA]=c2[DomB];

  if printOutput {
    writeln("Example 15) object A = B");
  }
  for (p1,p2) in zip(c1[DomA],c2[DomB]) do if (p1!=p2) then {writeln("ERROR!!!!");e=true;};
  if printOutput {
    writeln();
  }
 
  if e then writeln("Hey man!!!!... double check it! There was an ERROR!");
  proc LocaleDistribution(X:[]){
    var BA : [X.domain] int;
    forall ba in BA do
      ba = here.id;
    writeln(BA);
  }
}
