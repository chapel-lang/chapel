use Math;

// Type tests
typetest(complex(64));
typetest(complex(128));
typetest(imag(32));
typetest(imag(64));
typetest(real(32));
typetest(real(64));
typetest(int(32));
typetest(int(64));
typetest(uint(8));
typetest(uint(16));
typetest(uint(32));
typetest(uint(64));


/* Check that input type always matches output type */
proc typetest(type t) {
  var input: t;
  var output = conjg(input);
  if input.type != output.type {
    writeln('Failed on type ', input.type:string,
            ', received: ', output.type:string);
  }
}
