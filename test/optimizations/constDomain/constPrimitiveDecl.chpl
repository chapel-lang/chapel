// This shows a case where we shouldn't add FLAG_CONST to the init_coerce_tmp
// created
var s$: sync int;
s$ = 3;
const i: int = s$;
writeln(i);
