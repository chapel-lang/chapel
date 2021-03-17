module sizedTypes {
  config param w: int;
  var fullInt: int(w);
  var halfInt: int(w/2);

  var fullReal: real(w);
  var halfReal: real(w/2);

  var fullImag: imag(w);
  var halfImag: imag(w/2);

  var fullComp: complex(w);
  var halfComp: complex(w/2);
}
