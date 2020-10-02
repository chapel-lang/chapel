{
    var dom = {1..3,1..3,1..5};
    var x = dom.orderToIndex(27);
    writeln(x);
}
{
    var dom = {1..3,1..3,1..5 by -2};
    var x = dom.orderToIndex(10);
    writeln(x);
}
{
    var dom = {1..4 by 2,1..4 by 2,1..4 by -2};
    var x = dom.orderToIndex(6);
    writeln(x);
}
{
    var dom = {1..4 by 2,1..4 by 2 align 2,1..4 by -2};
    var x = dom.orderToIndex(5);
    writeln(x);
}
{
    var dom = {1..3,1..3};
    var x = dom.orderToIndex(11);
    writeln(x);
}