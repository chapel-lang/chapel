proc output(A : [1..10] numeric) {
    writeln(A);
}

var B : [1..10] int = [i in 1..10] i*2;
output(B);

