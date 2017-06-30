
class CC { var x: int; }
extern class EE { var y: real; }
record RR { var z: string; }

compilerWarning(isExternClassType(CC):string);
compilerWarning(isExternClassType(EE):string);
compilerWarning(isExternClassType(RR):string);
compilerWarning(isExternClassType(string):string);
compilerWarning(isExternClassType(int):string);
compilerError("done");
