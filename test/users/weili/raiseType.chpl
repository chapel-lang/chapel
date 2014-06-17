proc raiseType(type x)
{
        if (x==bool) return int;
        if (x==int) return real;
}

writeln(typeToString(raiseType(bool)));
writeln(typeToString(raiseType(int)));
