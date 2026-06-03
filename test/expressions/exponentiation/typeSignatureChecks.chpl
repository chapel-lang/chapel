use Types;

type expTypes = (int(8), int(16), int(32), int(64),
                 uint(8), uint(16), uint(32), uint(64),
                 real(32), real(64));

for param i in 0..<expTypes.size {
  for param j in 0..<expTypes.size {
    {
      type baseType = expTypes(i);
      type expType = expTypes(j);
      param base: baseType = 2;
      param exp: expType  = 3;
      param res = base**exp;
      writeln("param ", baseType:string, " **  param ", expType:string, " = param ", res, ": ", res.type:string);
    }
    {
      type baseType = expTypes(i);
      type expType = expTypes(j);
      param base: baseType = 2;
      const exp: expType  = 3;
      const res = base**exp;
      writeln("param ", baseType:string, " **  ", expType:string, " = ", res, ": ", res.type:string);
    }
    {
      type baseType = expTypes(i);
      type expType = expTypes(j);
      const base: baseType = 2;
      param exp: expType  = 3;
      const res = base**exp;
      writeln(baseType:string, " **  param ", expType:string, " = ", res, ": ", res.type:string);
    }
    {
      type baseType = expTypes(i);
      type expType = expTypes(j);
      const base: baseType = 2;
      const exp: expType  = 3;
      const res = base**exp;
      writeln(baseType:string, " **  ", expType:string, " = ", res, ": ", res.type:string);
    }
  }
}
    
  
