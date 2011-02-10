proc raiseType(type x) type
{
        if (x==bool) then return int;
        if (x==int) then return real;
}

writeln(typeToString(raiseType(bool)));
writeln(typeToString(raiseType(int)));
