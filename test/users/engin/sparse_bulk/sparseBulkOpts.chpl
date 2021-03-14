use LayoutCS;
use Random;

config const N = 8;
const dimRange = 0..#N;
const ParentDom = {dimRange, dimRange};

enum layoutTypes {coo, csr, csc};
config param layoutType = layoutTypes.coo;

var csrDom: sparse subdomain(ParentDom) dmapped CS(compressRows=true);
var cscDom: sparse subdomain(ParentDom) dmapped CS(compressRows=false);
var cooDom: sparse subdomain(ParentDom);

var SparseDom = if layoutType == layoutTypes.csr then 
                  csrDom
               else if layoutType == layoutTypes.csc then
                  cscDom
               else
                  cooDom;

var SparseMat: [SparseDom] int;

//left diagonal
var inds1: [{dimRange}] 2*int;
for i in dimRange do inds1[i] = (i,i);

shuffle(inds1);

var copyInds1 = inds1;

SparseDom.bulkAdd(inds1, false, true, true);
for i in inds1 do SparseMat[i] = 1;

writeln("After bulkAdd( , false, true, true)");
print();
checkIntegrity(inds1, copyInds1);

//right diagonal
var inds2: [{dimRange}] 2*int;
for i in dimRange do inds2[N-1-i] = (i,N-1-i);

SparseDom.bulkAdd(inds2, false, true, false);
for i in inds2 do SparseMat[i] = 2;

writeln("After bulkAdd( , false, true, false)");
print();

//column with duplicate indices
var inds3: [{0..#2*N}] 2*int;
for i in dimRange {
  inds3[2*i] = (i,3);
  inds3[2*i+1] = (i,3);
}

shuffle(inds3);

var copyInds3 = inds3;
SparseDom.bulkAdd(inds3, false, false, true);
for i in inds3 do SparseMat[i] = 3;

writeln("After bulkAdd( , false, false, true)");
print();
//check integrity
checkIntegrity(inds3, copyInds3);

//row with duplicate indices
var inds4: [{0..#2*N}] 2*int;
for i in dimRange {
  inds4[2*i] = (4,i);
  inds4[2*i+1] = (4,i);
}

shuffle(inds4);

SparseDom.bulkAdd(inds4, false, false, false);
for i in inds4 do SparseMat[i] = 4;

writeln("After bulkAdd( , false, false, false)");
print();

proc print() {
  for i in ParentDom.dim(0) {
    for j in ParentDom.dim(1) {
      write(SparseMat[i,j], " ");
    }
    writeln();
  } 
}

//column with duplicate indices -- sorted
var inds5: [{0..#2*N}] 2*int;
for i in dimRange {
  inds5[2*i] = (i,5);
  inds5[2*i+1] = (i,5);
}

var copyInds5 = inds5;
SparseDom.bulkAdd(inds5, true, false);
for i in inds5 do SparseMat[i] = 5;

writeln("After bulkAdd( , true, false)");
print();
//check integrity
checkIntegrity(inds5, copyInds5);

//row  -- sorted nodup
var inds6: [{0..#N}] 2*int;
for i in dimRange {
  inds6[i] = (6, i);
}

var copyInds6 = inds6;
SparseDom.bulkAdd(inds6, true, true);
for i in inds6 do SparseMat[i] = 6;

writeln("After bulkAdd( , true, true)");
print();
//check integrity
checkIntegrity(inds6, copyInds6);

proc checkIntegrity(a1, a2){
  for (i1, i2) in zip(a1, a2) do 
    if i1!=i2 then halt("broken");
}
