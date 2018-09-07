
iter ITR() { if numLocales < 0 then yield 555; }

var BRR = for idx in ITR() do [idx];
compilerWarning(BRR.type:string);
writeln(BRR.domain);
