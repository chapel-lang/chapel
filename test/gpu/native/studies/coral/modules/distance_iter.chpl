inline proc calc_distance(Array : [] real(32), Mask1 : ?, Mask2 : ?,
                          inout Output_tmp : real(64), param bs : int,
                          param be : int, i : int, j : int) {
  for (k,l) in Mask1 {
    for (m,n) in Mask2 {

      var dist : real(32) = 0;

      var tmp : real(32) = 0;
      for param p in bs..be {
        tmp += (Array[p,j+l,i+k]-Array[p,j+n,i+m])**2;
      }

      Output_tmp += sqrt(tmp);
    }
  }
}
