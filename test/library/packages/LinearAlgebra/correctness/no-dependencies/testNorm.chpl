use LinearAlgebra, Random;

{
    var A: [1..3, 1..3] real;
    
    A[1,..] = [1.0, 0.0, 0.0];
    A[2,..] = [1.0, 2.0, 0.0];
    A[3,..] = [1.0, 3.0, 5.0];

    const frobNorm = norm(A, normType.normFrob);
    const l1Norm = norm(A, normType.norm1);
    const linfNorm = norm(A, normType.normInf);
    
    writeln(frobNorm);
    writeln(l1Norm);
    writeln(linfNorm);
}

{
    var A: [1..3, 1..3] real;
    A = 0.0;

    const frobNorm = norm(A, normType.normFrob);
    const l1Norm = norm(A, normType.norm1);
    const linfNorm = norm(A, normType.normInf);
    
    writeln(frobNorm);
    writeln(l1Norm);
    writeln(linfNorm);
}

{
    var b: [1..3] real;

    fillRandom(b, seed=10);

    const frobNorm = norm(b, normType.normFrob);
    const l1Norm = norm(b, normType.norm1);
    const l2Norm = norm(b, normType.norm2);
    const linfNorm = norm(b, normType.normInf);

    writeln(frobNorm);
    writeln(l1Norm);
    writeln(l2Norm);
    writeln(linfNorm);
}

{
    var b: [1..3] real;

    b = 0.0;

    const frobNorm = norm(b, normType.normFrob);
    const l1Norm = norm(b, normType.norm1);
    const l2Norm = norm(b, normType.norm2);
    const linfNorm = norm(b, normType.normInf);

    writeln(frobNorm);
    writeln(l1Norm);
    writeln(l2Norm);
    writeln(linfNorm);
}
