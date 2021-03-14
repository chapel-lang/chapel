param two = 2.0;
param four = 4.0;
param twoi:imag = 2.0i;
param fouri:imag = 4.0i;

proc checkmult(param a, param b) {
  param prod = a * b;
  var avar = a;
  var bvar = b;
  var prodvar = avar * bvar;
  writeln(prod.type:string, " ", prod, " ", prod:string, " ",
          prodvar.type:string, " ", prodvar);
}
proc checkdiv(param a, param b) {
  param quot = a / b;
  var avar = a;
  var bvar = b;
  var quotvar = avar / bvar;
  writeln(quot.type:string, " ", quot, " ", quot:string, " ",
          quotvar.type:string, " ", quotvar);
}


// real real
checkmult(two, four);
checkdiv(four, two);

// imag imag
checkmult(twoi, fouri);
checkdiv(fouri, twoi);

// real imag
checkmult(two, fouri);
checkdiv(four, twoi);

// imag real
checkmult(twoi, four);
checkdiv(fouri, two);
