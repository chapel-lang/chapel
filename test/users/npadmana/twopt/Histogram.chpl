module Histogram {

  class UniformBins {
    param dim : int;
    var nbins : dim*int;
    var Dhist : domain(dim);
    var lo, hi,dx, invdx : [1..dim] real;
    var arr : [Dhist] atomic real;

    proc UniformBins(param dim : int, nbins : dim*int, limits : dim*(real,real)) {
      var dd : dim*range;
      this.nbins = nbins;
      for param ii in 1..dim {
        lo[ii] = limits(ii)(1);
        hi[ii] = limits(ii)(2);
        dx[ii] = (hi[ii]-lo[ii])/nbins(ii);
        dd(ii) = 0.. #nbins(ii);
      }
      invdx = 1.0/dx;
      Dhist = {(...dd)};
      [x in arr] x.write(0.0);
    }

    proc reset() {
      [x in arr] x.write(0.0);
    }

    proc bins(idim : int) {
      var bounds : [0.. #nbins(idim)+1] real;
      for i in {0.. #(nbins(idim)+1)} do bounds[i] = lo[idim] + dx[idim]*i;
      return bounds;
    }

    proc add(x : dim*real, w : real) {
      for param ii in 1..dim do
        if ((x(ii) < lo[ii]) | (x(ii) >= hi[ii])) then return;

      var pos : dim*int;
      for param ii in 1..dim do pos(ii) = ((x(ii)-lo(ii))*invdx[ii]) : int;
      arr[pos].add(w);
    }

    proc set(ndx : dim*int, val : real) {
      arr[ndx].write(val);
    }

    proc this(ndx) : real {
      return arr[ndx].read();
    }

  } // UniformBins

  proc writeHist(ff : channel, hh : UniformBins, fmt : string = "%20.14er ") {
    // Dump out values
    for xx in hh.bins(1) do ff.writef("%12.4dr",xx); 
    ff.writeln();
    for xx in hh.bins(2) do ff.writef("%12.4dr",xx); 
    ff.writeln("\n##");
    for ii in hh.Dhist.dim(1) {
      for jj in hh.Dhist.dim(2) {
        ff.writef(fmt, hh[(ii,jj)]);
      }
      ff.writeln();
    }
  }


} // End module Histogram
