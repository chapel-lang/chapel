def f(x) {
    if x == 0 {
        var y : [1..10] int;
        return y;

    } else 
        /* return */ f(x-1);
}

var x = f(10);
