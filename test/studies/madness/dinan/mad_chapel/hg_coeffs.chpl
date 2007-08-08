module hg {
  config const hg_inputfile = "hg_coeffs.dat";
  var hg_maxK: int;

  // FIXME: This should be module-private
  var hg_coeffs: [1..0, 1..0, 1..0] real;

  
  def hg_getCoeffs(k: int) {
    var kthCoeffs: [0..2*k-1, 0..2*k-1] real;
    // FIXME: Ideally I want to slice a 2d matrix out of the 3d matrix.
    //        Current copy code is a workaround.
    // var kthCoeffs: [1..2*k, 1..2*k] real = coeffs[k, 1..2*k, 1..2*k];

    for (i, j) in kthCoeffs.domain do 
      kthCoeffs(i, j) = hg_coeffs(k, i, j);

    return kthCoeffs;
  }

  
  def hg_readCoeffs(inputfile) {
    var max_k: int;
    var coeffData = file(inputfile, path='./', mode='r');

    coeffData.open();
    coeffData.read(max_k);
    hg_maxK = max_k;

    hg_coeffs.domain = [0..max_k, 0..2*max_k-1, 0..2*max_k-1];
    writeln("HG: Reading ", max_k, " sets of twoscale coeffs from: ", inputfile, "\n");

    // hg_coeffs[0, , ] is not used!
    for i in 1..max_k {
      for (j, k) in [0..i*2-1, 0..i*2-1] {
        var x: real;
        coeffData.read(x);
        hg_coeffs(i, j, k) = x;
      }
    }

    coeffData.close();

    //writeln("Read in coeffs: ", hg_coeffs.domain);
    //for i in 1..max_k do
    //  writeln("hg[", i, "]=\n", hg_getCoeffs(i));
  }


  // --- Initialization --- //
  hg_readCoeffs(hg_inputfile);
}
