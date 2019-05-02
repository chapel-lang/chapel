config const s1=2, s2=2;
const mygriddom = {0..#s1, 0..#s2};
const mygridlocs: [mygriddom] locale = Locales(0);

use BlockDist;
config const n=500;
var DA = {1..n,1..n};
var Dom1 = DA dmapped Block(DA);
var C:[Dom1] real(64);
var D5 = {1..10, 1..10 by 2};
var D6 = {1..10, 1..5};
var D3 = {101..400 by 2,51..350 by 3};
var H: [1..n,1..n] real(64);

writeln(" DR defining in ",{1..n,1..n});
writeln(" BD defining in ",{1..n,1..n});
writeln();

C = [(i,j) in Dom1] (i-1)*n + j;
H = [(i,j) in H.domain] (i-1)*n + j + 499;

writeln("Example 1: DR",D6," = BD",D6);
H[D6] = C[D6];

D6={1..10,1..n/2};
writeln();

for (a,i) in zip(H,{1..n*n}) do a=i;
writeln("Example 2: DR",D6," = BD",D6);
H[D6] = C[D6];
for (a,b) in zip(H[D6],C[D6]) do if (a!=b) then writeln("ERROR!!!!");

D6={1..10,1..3*n/4};
writeln();
writeln("Example 3: DR",D6," = BD",D6);
H[D6] = C[D6];
for (a,b) in zip(H[D6],C[D6]) do if (a!=b) then writeln("ERROR!!!!");

D3={1..10,1..3*n/4 by 6};
D5={1..20 by 2,1..n/4 by 2};
writeln();
for (a,i) in zip(H,{1..n*n}) do a=i;
writeln("Example 4: DR",D3," = BD",D5);
H[D3] = C[D5];
for (a,b) in zip(H[D3],C[D5]) do if (a!=b) then writeln("ERROR!!!!");

D3={1..10,1..3*n/4 by 6};
D5={1..20 by 2,1..n/4 by 2};
writeln();
for (a,i) in zip(H,{1..n*n}) do a=i;
writeln("Example 5: DR",D3," = BD",D5);
H[D3] = C[D5];
for (a,b) in zip(H[D3],C[D5]) do if (a!=b) then writeln("ERROR!!!!");

D3={1..3*n/4 by 3,1..3*n/4 by 6};
D5={n/2+1..n by 2,1..n/4 by 2};
writeln();
for (a,i) in zip(H,{1..n*n}) do a=i;
writeln("Example 6: DR",D3," = BD",D5);
H[D3] = C[D5];
for (a,b) in zip(H[D3],C[D5]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
for (a,i) in zip(H,{1..n*n}) do a=i;
writeln("Example 7: DR",D5," = BD",D3);
H[D5] = C[D3];
for (a,b) in zip(H[D5],C[D3]) do if (a!=b) then writeln("ERROR!!!!");

D3={1..n by 2,1..n by 4};
D5={1..n/2,1..n/2 by 2};
writeln();
for (a,i) in zip(H,{1..n*n}) do a=i;
writeln("Example 8: DR",D3," = BD",D5);
H[D3] = C[D5];
for (a,b) in zip(H[D3],C[D5]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
writeln("Example 9: DR",D5," = BD",D3);
for (a,i) in zip(H,{1..n*n}) do a=i;
H[D5] = C[D3];
for (a,b) in zip(H[D5],C[D3]) do if (a!=b) then writeln("ERROR!!!!");

D3={1..n/2 ,1..n/4};
D5={1..n/2,1..n/2 by 2};
writeln();
writeln("Example 10: DR",D5," = BD",D3);
for (a,i) in zip(H,{1..n*n}) do a=i;
H[D5] = C[D3];
for (a,b) in zip(H[D5],C[D3]) do if (a!=b) then writeln("ERROR!!!!");

D3={1..n/4 ,1..n/2};
D5={1..n by 4,1..n/2};
writeln();
writeln("Example 11: DR",D5," = BD",D3);
for (a,i) in zip(H,{1..n*n}) do a=i;
H[D5] = C[D3];
for (a,b) in zip(H[D5],C[D3]) do if (a!=b) then writeln("ERROR!!!!");

writeln();
writeln("Example 12: DR",D3," = BD",D5);
for (a,i) in zip(H,{1..n*n}) do a=i;
H[D3] = C[D5];
for (a,b) in zip(H[D3],C[D5]) do if (a!=b) then writeln("ERROR!!!!");

D3={1..n/2 ,1..n/4};
D5={1..n/2,1..n/2 by 2};
writeln();
writeln("Example 13: DR",D3," = BD",D5);
for (a,i) in zip(H,{1..n*n}) do a=i;
H[D3] = C[D5];
for (a,b) in zip(H[D3],C[D5]) do if (a!=b) then writeln("ERROR!!!!");

//writeln(" DR: ", H);
//writeln(" BD: ", C);
