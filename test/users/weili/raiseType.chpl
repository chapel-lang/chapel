proc raiseType(type x)
{
        if (x==bool) return int;
        if (x==int) return real;
}

writeln(raiseType(bool):string);
writeln(raiseType(int):string);
