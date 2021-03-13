use LinearAlgebra;
use Random;
use TestUtils;

{
    var A = Matrix(
        [1.0, 3.0, 2.0],
        [5.0, 2.0, 0.0]
    );
    var B = pinv(A);

    var npB = Matrix(
        [-0.09122807,  0.20701754],
        [ 0.22807018, -0.01754386],
        [ 0.20350877, -0.07719298]
    );

    assertAlmostEqual(npB, B, "real: pinv(A)");

    var Acalc = dot(A, dot(B, A));

    assertAlmostEqual(Acalc, A, "real: A * pinv(A) * A");
}

{
    var A = Matrix(
        [18.0, 22.0, 54.0],
        [42.0, 22.0, 70.0],
        [86.0, 62.0, 54.0],
        [86.0, 174.0, 134.0],
        [42.0, 62.0, 134.0],
        [53.0, 88.0, 93.0]
    );

    var npB = Matrix(
        [-0.00088878,  0.00574856,  0.01593542, -0.00438144, -0.00403594,
            -0.00093537],
        [-0.00229485, -0.00650944, -0.00441327,  0.00906565, -0.00409677,
            0.00163516],
        [ 0.00337342,  0.00348255, -0.00419068, -0.00267184,  0.00805934,
            0.00084332]
    );

    var B = pinv(A);
    assertAlmostEqual(npB, B, "real: pinv(A)");

    var Acalc = dot(A, dot(B, A));

    assertAlmostEqual(Acalc, A, "real: A * pinv(A) * A");
}

//For complex types
{
    var A = Matrix(
        [1.0+1.0i, 2.0+1.0i, 3.0+1.0i],
        [3.0-2.0i, 2.0+1.0i, 4.0+1.0i],
        [1.0+1.0i, 2.0+1.0i, 5.0+1.0i], eltType=complex(128)
    );

    var B = pinv(A);

    var npB = Matrix(
        [-7.69230769e-02-1.15384615e-01i,  1.53846154e-01+2.30769231e-01i,
            -7.69230769e-02-1.15384615e-01i],
        [ 1.12307692e+00-2.15384615e-01i, -4.61538462e-02-1.69230769e-01i,
            -6.76923077e-01+1.84615385e-01i],
        [-5.00000000e-01+5.55111512e-16i, -7.59016391e-18-3.82679483e-16i,
            5.00000000e-01-4.16333634e-17i], eltType = complex(128)
    );

    assertAlmostEqual(npB, B, "complex: pinv(A)");

    var Acalc = dot(A, dot(B, A));

    assertAlmostEqual(Acalc, A, "complex: A * pinv(A) * A");
}

{
    var A = Matrix(
        [4.3 + 2.2i, 5.3 - 5i],
        [7.6 - 3.8i, 8.8 + 4i],
        [5.5 -3.2i, 4.8 + 6.3i],
        [8.2 + 2.6i, 9.9 + 2i], eltType = complex(64)
    );

    var B = pinv(A);

    var npB = Matrix(
        [ 0.02416431-0.05132241i,  0.01967425+0.03283327i,  0.01428943+0.04557465i,
            0.02406656-0.0335065i],
        [ 0.01667378+0.04733845i,  0.01087928-0.02480544i, -0.00219163-0.03981815i,
            0.02741716+0.01589353i], eltType = complex(64)
    );

    assertAlmostEqual(npB, B, "complex: pinv(A)");

    var Acalc = dot(A, dot(B, A));

    assertAlmostEqual(Acalc, A, "complex: A * pinv(A) * A");
}