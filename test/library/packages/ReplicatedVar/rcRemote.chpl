use ReplicatedVar;
var a: [rcDomain] int;
rcReplicate(a, 10);

rcRemote(a, Locales[numLocales-1]) = 14;

var collected: [LocaleSpace] int;
rcCollect(a, collected);
writeln(collected);
