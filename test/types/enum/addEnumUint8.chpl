enum E {a=1};
param Ea = 1;
param one8 = 1:uint(8);

compilerWarning((Ea+one8).type:string);
compilerWarning((E.a:int(64)+one8).type:string);
writeln((E.a+one8).type:string);
