config var I=10, J=10, K=10;
config var bi=5, bj=6, bk=7;
config var limit=10;
var i=I, j=J, k=K, l=limit;
var ii=I, jj=J, kk=K;

do {
  label l1 for i in 0..I by -1 {
    ii -= 1;
    if (i == bi) then continue l1;

    label l2 for j in 0..J by -1 {
      jj -= 1;
      if (j == bj) then continue l2;
      if (j == bi) then continue l1;

      label l3 for k in 0..K by -1 {
        kk -= 1;
	if (k == bk) then continue l3;
	if (k == bj) then continue l2;
	if (k == bi) then continue l1;
      }
    }
  }
  l +=1;
 } while (l < limit);


writeln((ii, jj, kk));
