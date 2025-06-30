use BigInteger;
/*START_EXAMPLE*/
// These would result in integer overflow and cause compile-time errors
var bad1 = 4847382292989382987395534934347: bigint;
var bad2 = new bigint(4847382292989382987395534934347);

// These give the desired result
var good1 = "4847382292989382987395534934347": bigint;
var good2 = new bigint("4847382292989382987395534934347");
/*STOP_EXAMPLE*/
