class Foo {
    type eltType;
    var elements : [1..3] eltType;

    proc Foo (type eltType, first : eltType, second : eltType, third : eltType)
    {
        elements[1] = first;
        elements[2] = second;
        elements[3] = third;
    }

    iter these ref : eltType {
        for el in elements do
            yield el;
    }
};

var f = new Foo(string, "one", "two", "three");
for el in f do
    writeln(el);

