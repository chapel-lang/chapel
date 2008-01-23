module blockindices {
use fock;

class blockIndices {
  const is, js, ks, ls: range(int);

  def blockIndices(iat, jat, kat, lat) {
    is = bas_info(iat, 1)..bas_info(iat, 2);
    js = bas_info(jat, 1)..bas_info(jat, 2);
    ks = bas_info(kat, 1)..bas_info(kat, 2);
    ls = bas_info(lat, 1)..bas_info(lat, 2);
  }

  // BLC: TODO -- this should have a better name
  def genIndices {
    for i in is {
      const jtop = if (is.low==js.low) then i else js.high;
      for j in js.low..jtop {
        const facij = if (i==j) then 0.5 else 1.0;
        const ktop = if (is.low==ks.low && js.low==ls.low) then i else ks.high;
        for k in ks.low..ktop {
          var ltop = ls.high;
          if (ks.low==ls.low) then ltop = k;
          if (is.low==ks.low && js.low==ls.low && k==i) then ltop = j;
          for l in ls.low..ltop {
            var facijkl = facij;
            if (k==l) then facijkl *= 0.5;
            if (i==k && j==l) then facijkl *= 0.5;
            const gijkl = g(i,j,k,l)*facijkl;

            yield (i,j,k,l,gijkl);
          }
        }
      }
    }

  }
}

}