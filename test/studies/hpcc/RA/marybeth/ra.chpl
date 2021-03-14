/*  This is the Chapel version of Random Access HPC benchmark.
 *
 *  For now, memory size is a compile time parameter.  That will
 *  be changed to a runtime parameter later.
 *
 *  Timing is not implemented yet.  So, the GUPs number is not 
 *  computed.
 *  
 *  This program performs the updates to Table and checks if they
 *  are correct.
 *
 *  This is a pretty straightforward translation from C.  More changes
 *  will be made to make it a true Chapel program.
 *
 *  last revised 9/18/2008 by marybeth
 */  
param POLY:uint(64) = 0x0000000000000007;
param PERIOD:int(64) = 1317624576693539401;
param MEMSIZE:int = 1000; 

var NUPDATE:int;

proc HPCC_starts(n:int(64)):uint(64) {
  var i:int;
  var j:int;
  var n2 = n;
  var temp:uint(64);
  var ran:uint(64);
  var D: domain(1) = {0..63};
  var m2: [D] uint(64);
  var result: uint(64);

  while (n2 < 0) do {
    n2 += PERIOD;
  }
  while (n2 > PERIOD) do {
    n2 -= PERIOD;
  }
  if (n2 == 0) {
    result = 0x1;
    return result;
  }

  temp = 0x1;
  for i in D {
    m2(i) = temp;
    if (temp:int(64)) < 0 then
      temp = ((temp << 1) ^ POLY);
    else
      temp = ((temp << 1) ^ 0);
    if (temp:int(64)) < 0 then
      temp = ((temp << 1) ^ POLY);
    else
      temp = ((temp << 1) ^ 0);
  }

  i = 62;
  while (((n2 >> i) & 1) == 0) do i -= 1;

  ran = 0x2;
  while (i > 0) do {
    temp = 0;
    for j in D do {
      if ((ran >> j:uint(64)) & 1) then temp ^= m2(j);
    }
    ran = temp;
    i -= 1;
    if ((n2 >> i) & 1) then {
      if (ran:int(64)) < 0 then
        ran = ((ran << 1) ^ POLY);
      else
        ran = ((ran << 1) ^ 0);
    }
  }
  return ran;
}

proc RandomAccessUpdate(TableSize:uint(64), Table: [] uint(64)) {
  var i:int;
  var D: domain(1) =  {0..127};
  var ran: [D] uint(64);              
  var j:int;

  for i in Table.domain {
    Table(i) = i:uint(64);
  }
  for j in D {
    ran(j) = HPCC_starts (((NUPDATE/128) * j):int(64));
  }

  i = 0;
  do {
    for j in D {
      if (ran(j):int(64)) < 0 then
        ran(j) = ((ran(j) << 1) ^ POLY);
      else
        ran(j) = ((ran(j) << 1) ^ 0);
      Table((ran(j) & (TableSize-1)):int) ^= ran(j);
    }
    i += 1;
  } while (i < NUPDATE/128);
}

proc main() {

  var i:int;
  var temp:uint(64);
  var cputime:real;               /* CPU time to update table */
  var realtime:real;              /* Real time to update table */
  var GUPs: real;
  var failure:int;
  var totalMem:real = (MEMSIZE:real)/8.0;
  var TableSize:uint(64);
  var logTableSize:uint(64); 

  /* calculate local memory per node for the update table */
  totalMem = (MEMSIZE:real)/(8.0);

  /* calculate the size of update array (must be a power of 2) */
  totalMem *= 0.5;
  logTableSize = 0;
  TableSize = 1;
  while (totalMem >= 1.0) {
   totalMem *= 0.5;
   logTableSize += 1;
   TableSize = TableSize << 1;
  }

  const TableDomain = TableSize:int;
  var D:domain(1) = {0..TableDomain-1};
  var Table: [D] uint(64);

  NUPDATE = 4*(TableSize:int);

  writeln("Main table size = 2^",logTableSize," = ",TableSize," words");
  writeln("Number of updates = ",NUPDATE);

  RandomAccessUpdate( TableSize, Table );

  /* Verification of results (in serial or "safe" mode; optional) */
  i  = 0;
  temp = 0x1;
  do {
    if (temp:int(64)) < 0 then
      temp = ((temp << 1) ^ POLY);
    else
      temp = ((temp << 1) ^ 0);
    Table((temp & (TableSize-1)):int) ^= temp;
    i += 1;
  } while (i < NUPDATE);

  temp = 0;
  for i in D {
    if (Table[i] != i:uint(64)) then temp += 1;
  }

  writeln("Found ", temp, " errors in ", TableSize, " locations");
  if (temp <= 0.01*TableSize) then 
    writeln("(passed)");
  else
    writeln("(failed)");
}
