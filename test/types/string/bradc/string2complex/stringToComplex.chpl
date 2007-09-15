var strarr: [1..17] string = ("1.2+3.4i",
                              "2.3-4.5i",
                              "3.4 + 5.6i",
                              "4.5 - 6.7i",
                              "5.6+ 7.8i",
                              "6.7- 8.9i",
                              "7.8 +9.0i",
                              "8.9 -0.1i",
                              "9.0 + -1.2i",
                              "0.1 + +2.3i",
                              "1.2--3.4i",
                              "2.3",
                              "+3.4",
                              "-4.5",
                              "5.6i",
                              "+6.7i",
                              "-7.8i"
                              );

for i in strarr.domain {
  const c64 = strarr(i):complex(64);
  const c128 = strarr(i):complex(128);
  writeln("c64 =  ", c64);
  writeln("c128 = ", c128);
}


