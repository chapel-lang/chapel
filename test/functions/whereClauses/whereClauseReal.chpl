var ax: [1..10] real = 1.0;
var yc = wrong();
proc wrong(): real
  where (ax.rank == 1): real {
   return 0.0;
}
