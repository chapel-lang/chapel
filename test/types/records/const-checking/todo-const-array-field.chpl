// Test errors when a const field is of an array type.

const DDD = {1..2};

record RecordWithVarField {
  var AAA: [DDD] int;
}

record RecordWithConstField {
  const CCC: [DDD] int;
}

const rwVF: RecordWithVarField;
rwVF.AAA = [3,4];  // error
rwVF.AAA[1] = 2;   // error

var rwCF: RecordWithConstField;
rwCF.CCC = [6,7];  // error
rwCF.CCC[1] = 5;   // error

compilerError("done"); // no need to do anything else
