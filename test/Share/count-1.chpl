var strAlphabet [1..26**2] =
        reshape([i in 'A'..'Z'][j in 'A'..'Z'] i+j);
writeln("expect 676 ", strAlphabet.count);
