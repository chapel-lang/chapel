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
const POLY:uint(64) = 7u;

config const verify = true;

config const TotalMemSize:int = 2000;
config const LogTableSize:int  = lg((TotalMemSize/2.0):int);
const TableSize = (1 << LogTableSize):uint(64);
const NumUpdates = 4*TableSize;
const NumStreams = 1 << 6;
const BigStep = (NumUpdates/NumStreams):int;

const TableDomain = [0..(TableSize-1):int];
var Table: [TableDomain] uint(64);

const RandStepsDomain = [0..63];
var RandomSteps: [RandStepsDomain] uint(64);

const StreamDomain = [0..NumStreams-1];
const BigStepDomain = [0..BigStep-1];
// const UpdateDomain: domain(2) = [StreamDomain,BigStepDomain];
const UpdateDomain: domain(2) = [0..NumStreams-1,0..BigStep-1];


def main() {

  var Time:float;

  writeRAdata();

  InitRandomSteps();

  Time = Timer();
  RandomAccessUpdate();
  Time = Timer() - Time;

  if (verify) then VerifyResults();

  writeRAresults();

}

def RandomAccessUpdate() {

  [i in TableDomain] Table(i) = i:uint(64);
  
  for j in StreamDomain {
    var ran:uint(64) = RandomStart(BigStep*j);
    for i in BigStepDomain {
      ran = (ran << 1u) ^ (if (ran:int(64) < 0) then POLY else 0u);
      Table((ran & (TableSize-1)):int) ^= ran;
    }
  }
}

def RandomStart(step0:int):uint(64) {

  var i:int;
  var ran:uint(64) = 2u;

  if (step0 ==0) then 
    return 0x1u;
  else
    i = lg(step0);
  while (i > 0) do {
    var temp:uint(64) = 0u;
    [j in RandStepsDomain] if (( ran >> (j:uint(64))) & 1) then temp ^= RandomSteps(j);
    ran = temp;
    i -= 1;
    if (( step0 >> i) & 1) then
      ran = (ran << 1) ^ (if (ran:int(64) < 0) then POLY else 0u);
  }
  return ran;
}

def InitRandomSteps() {
  
  var temp:uint(64) = 1u;

  for i in RandStepsDomain {
    RandomSteps(i) = temp;
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0u);
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0u);
  }
}

def VerifyResults() {
  config const ErrorTolerance = 0.01;

  var temp: uint(64) = 1u;  
  for i in UpdateDomain {
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0u);
    Table((temp & (TableSize-1)):int) ^= temp;  
  }

  var NumErrors = 0;
  for i in TableDomain {
    if (Table(i) != i:uint(64)) then
      NumErrors += 1;
  }

  write("Found ", NumErrors, " errors in ", TableSize, " locations ");
  if (NumErrors <= ErrorTolerance*TableSize) {
    writeln("(passed)");
  } else {
    writeln("(failed)");
  }
}

def Timer():float {
  return 1.0;
}

def writeRAdata() {
  writeln("Input data");
}

def writeRAresults() {
  writeln("GUPs number");
}
