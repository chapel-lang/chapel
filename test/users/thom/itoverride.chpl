
class C
{
    proc writeThis(w:Writer) { w.write("C"); }
}

class SubC : C
{
    proc writeThis(w:Writer) { w.write("SubC"); }
}

class OverrideMe
{
    proc getC()
    {
        return new C();
    }

    iter manyC()
    {
        yield new C();
        yield new C();
    }
}

class OverridesIt : OverrideMe
{
    proc getC()
    {
        return new SubC();
    }
    
    iter manyC()
    {
        //even if these are C() this still breaks
        yield new SubC();
        yield new SubC();
    }
}

proc main()
{
    var o : OverrideMe;
    o = new OverridesIt();
    writeln("Should be subC: ",o.getC());

    //code compiles without this line
    writeln("Many: ",o.manyC());
    delete o;
}
