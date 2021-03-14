proc foo() : ([1..3] int, [1..3] int) {
    var A : [1..3] int = (1, 2, 3);
    var B : [1..3] int = (4, 5, 6);

    return (A, B);
}

writeln(foo());

