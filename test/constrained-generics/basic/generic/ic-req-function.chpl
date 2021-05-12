// interface with an IC function

interface HLP { }

interface IFC {
  proc reqFn(formal: ?T) where T implements HLP;
}

int implements IFC;
real implements HLP;

proc reqFn(formal: ?R) where R implements HLP {
  writeln("in reqFn.R");
}

proc icFun(arg1: ?Q1, arg2: ?Q2)
  where arg1 implements IFC && arg2 implements HLP
{
  reqFn(arg2);
}

icFun(55, 66.77);
