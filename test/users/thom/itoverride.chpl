
class C
{
    def writeThis(w:Writer) { w.write("C"); }
}

class SubC : C
{
    def writeThis(w:Writer) { w.write("SubC"); }
}

class OverrideMe
{
    def getC()
    {
        return new C();
    }

    def manyC()
    {
        yield new C();
        yield new C();
    }
}

class OverridesIt : OverrideMe
{
    def getC()
    {
        return new SubC();
    }
    
    def manyC()
    {
        //even if these are C() this still breaks
        yield new SubC();
        yield new SubC();
    }
}

def main()
{
    var o : OverrideMe;
    o = new OverridesIt();
    writeln("Should be subC: ",o.getC());

    //code compiles without this line
    writeln("Many: ",o.manyC());
}
