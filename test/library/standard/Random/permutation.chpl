use Random;

{
    var dom = {1..5};
    var arr :[1..10] real;

    permutation(dom,arr,seed=10);

    writeln(arr);
}

{
    var dom = {1..10 by 2};
    var arr: [dom] int;

    forall i in dom do
        arr[i] = i;
    
    permutation(dom,arr,seed=10);

    writeln(arr);
}

{
    var dom = {1..20 by -3};
    var arr: [1..10] real;

    permutation(dom,arr,seed=10);

    writeln(arr);
}

{
    var dom = {1..20 by 3};
    var arr: [1..<20] real;

    permutation(dom,arr,seed=10);

    writeln(arr);
}