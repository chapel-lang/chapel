
/*****
      ex_datapar.chpl -
      Examples of data parallelism: forall loop, reduction, and scan.
      See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

var img1 : [0..999, 0..999] int;        /* data to work on */
var colsumA : [img1.domain.dim(1)] int; /* partial sum over column */

writeln("\nDemonstrating forall and reduce");

forall (x,y) in img1.domain do
  img1(x,y) = (x**2) + (y**2);

/* This also works, using the square brackets notation in place of the forall.
img1 = [(x, y) in img1.domain] (x**2) + (y**2);
 */

forall (x,y) in img1.domain do
  colsumA(x) += img1(x,y);

var grandsum = + reduce colsumA;
writeln("  grand sum via column sum   ", grandsum);

/* Or as a one-liner. */
var grandsum2 = + reduce (forall x in img1.domain.dim(1) do
                            + reduce img1[x,img1.domain.dim(1)]);
writeln("  grand sum via 2x reduce    ", grandsum2);


writeln("\nDemonstrating scan");
var img2 : [0..4, 0..4] int;
var colsumB : [0..4][0..4] int;
var intimg : [0..4][0..4] int;

forall (i,j) in img2.domain do
  img2(i,j) = (i*5) + j;

writeln("source image");
writeln(img2);

forall j in 0..4 do
  colsumB(j) = + scan img2[0..4,j];

writeln("\nfirst scan (transposed cumulative column sum)");
for j in 0..4 do
  writeln(colsumB(j));

/* Transpose. */
forall j in 0..4 do
  for i in j+1..4 do
    colsumB[i][j] <=> colsumB[j][i];

forall i in 0..4 do
  intimg(i) = + scan colsumB[i][0..4];

writeln("\nintegral image");
for i in 0..4 do
  writeln(intimg(i));
