inline proc calc_distance(Array : [] real(32), Mask1 : ?, Mask2 : ?,
                          inout Output_tmp : real(64), bs : int, be : int,
                          i : int, j : int) {
  for (k,l) in Mask1 {
    for (m,n) in Mask2 {

      var dist : real(32) = 0;

      var tmp : real(32) = 0;
      for p in bs..be {
        tmp += (Array[p,i+k,j+l]-Array[p,i+m,j+n])**2;
      }

      Output_tmp += sqrt(tmp);
    }
  }
}
