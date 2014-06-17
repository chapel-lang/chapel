quadsol(3.0,8.0,5.0);
quadsol(3.0,4.0,5.0);

proc quadsol(a:real, b:real, c:real) {
  writeln("The solution of ",a,"x^2 + ",b,"x + ",c," = 0 is:");
  if (b*b > 4.0*a*c) {
    var x:  (real, real);

    x = let temp1 = sqrt(b*b - 4.0*a*c), temp2 = 2.0*a in
        ((-b + temp1)/temp2, (-b - temp1)/temp2);

    writeln(x);
  } else {
    var x: (complex, complex);

    x = let temp1 = sqrt(4.0*a*c - b*b)/(2.0*a), temp2 = -b/(2.0*a) in
        ((temp2,temp1):complex,(temp2,-temp1):complex);

    writeln(x);
  }
}
