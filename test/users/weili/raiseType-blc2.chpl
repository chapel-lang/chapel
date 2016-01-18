proc raiseType(type x) type
{
        if (x==bool) then return int;
        if (x==int) then return real;
}

writeln(raiseType(bool):string);
writeln(raiseType(int):string);
