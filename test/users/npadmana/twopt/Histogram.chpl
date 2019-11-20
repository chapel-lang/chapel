module Histogram {
  private use IO;

  // This allows us to turn off the atomic histogram add
  // to test the overhead we're getting from it.
  config param doHist = true;

  class UniformBins {
    param dim : int;
    var nbins : dim*int;
    var Dhist : domain(dim);
    var lo, hi,dx, invdx : [0..#dim] real;
    var arr : [Dhist] atomic real;

    proc init(param dim : int, nbins : dim*int, limits : dim*(real,real)) {
      this.dim = dim;
      this.complete();
      var dd : dim*range;
      this.nbins = nbins;
      for param ii in 0..dim-1 {
        lo[ii] = limits(ii)(0);
        hi[ii] = limits(ii)(1);
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
      for param ii in 0..dim-1 do
        if ((x(ii) < lo[ii]) | (x(ii) >= hi[ii])) then return;

      var pos : dim*int;
      for param ii in 0..dim-1 do pos(ii) = ((x(ii)-lo(ii))*invdx[ii]) : int;
      if doHist then arr[pos].add(w);
    }

    proc set(ndx : dim*int, val : real) {
      arr[ndx].write(val);
    }

    proc this(ndx) : real {
      return arr[ndx].read();
    }

  } // UniformBins

  proc writeHist(ff : channel, hh : borrowed UniformBins, fmt : string = "%20.14er ")
    throws {
    // Dump out values
    for xx in hh.bins(0) do ff.writef("%12.4dr",xx); 
    ff.writeln();
    for xx in hh.bins(1) do ff.writef("%12.4dr",xx); 
    ff.writeln("\n##");
    for ii in hh.Dhist.dim(0) {
      for jj in hh.Dhist.dim(1) {
        ff.writef(fmt, hh[(ii,jj)]);
      }
      ff.writeln();
    }
  }


} // End module Histogram
