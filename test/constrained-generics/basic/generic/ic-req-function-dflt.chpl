// interface with an IC function

interface HLP {
  proc write(arg: Self);
}

interface IFC {
  proc reqFn(formal: ?T) where T implements HLP {
    write("in IFC.reqFn  formal = "); write(formal); writeln();
  }
}

int implements IFC;
real implements HLP;

proc icFun(arg1: ?Q1, arg2: ?Q2)
  where arg1 implements IFC && arg2 implements HLP
{
  reqFn(arg2);
}

icFun(55, 66.77);
