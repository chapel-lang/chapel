class ReplaceVarConcrete {
  var newName: int;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName ref: int {
    return this.newName;
  }
}

record ReplaceVarConcrete2 {
  var newName: int;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName ref: int {
    return this.newName;
  }
}

class ReplaceType {
  type newName;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName type {
    return this.newName;
  }
}

record ReplaceType2 {
  type newName;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName type {
    return this.newName;
  }
}

class ReplaceParam {
  param newName;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName param {
    return this.newName;
  }
}

record ReplaceParam2 {
  param newName;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName param {
    return this.newName;
  }
}

class ReplaceConstConcrete {
  const newName: int;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName: int {
    return this.newName;
  }
}

record ReplaceConstConcrete2 {
  const newName: int;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName: int {
    return this.newName;
  }
}

class ReplaceVarGeneric {
  var newName;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName ref {
    return this.newName;
  }
}

record ReplaceVarGeneric2 {
  var newName;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName ref{
    return this.newName;
  }
}

class ReplaceConstGeneric {
  const newName;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName {
    return this.newName;
  }
}

record ReplaceConstGeneric2 {
  const newName;

  deprecated "The field 'oldName' is deprecated, please use 'newName'"
  proc oldName {
    return this.newName;
  }
}

proc main() {
  // Check various declarations of a class with the new field present
  var c1: ReplaceVarConcrete = new ReplaceVarConcrete(10);
  var c2 = new ReplaceVarConcrete();
  // Check direct references
  writeln(c1.newName);
  writeln(c1.oldName); // Should warn, but show the same value
  // Check that the two "fields" stay in sync if new field is updated
  c2.newName += 3;
  writeln(c2.newName);
  writeln(c2.oldName); // Should warn, but show the same value
  // Check that the two "fields" stay in sync if old "field" is updated
  c2.oldName -= 2; // Should warn
  writeln(c2.newName);
  writeln(c2.oldName); // Should warn, but show the same value

  // Check various declarations of a record with the new field present
  var r1: ReplaceVarConcrete2 = new ReplaceVarConcrete2(10);
  var r2 = new ReplaceVarConcrete2();
  var r3: ReplaceVarConcrete2;
  // Check direct references
  writeln(r1.newName);
  writeln(r1.oldName); // Should warn, but show the same value
  // Check that the two "fields" stay in sync if new field is updated
  r2.newName += 3;
  writeln(r2.newName);
  writeln(r2.oldName); // Should warn, but show the same value
  // Check that the two "fields" stay in sync if old "field" is updated
  r3.oldName -= 2; // Should warn
  writeln(r3.newName);
  writeln(r3.oldName); // Should warn, but show the same value

  // Check various declarations of a class with the new field present
  var c4: ReplaceType = new ReplaceType(uint);
  var c5: ReplaceType(int) = new ReplaceType(int);
  // Check direct references
  writeln(c4.newName: string);
  writeln(c4.oldName: string); // Should warn, but show the same value
  writeln(c5.newName: string);
  writeln(c5.oldName: string); // Should warn, but show the same value

  // Check various declarations of a record with the new field present
  var r4: ReplaceType2 = new ReplaceType2(uint);
  var r5 = new ReplaceType2(int);
  var r6: ReplaceType2(bool);
  // Check direct references
  writeln(r4.newName: string);
  writeln(r4.oldName: string); // Should warn, but show the same value
  writeln(r5.newName: string);
  writeln(r5.oldName: string); // Should warn, but show the same value
  writeln(r6.newName: string);
  writeln(r6.oldName: string); // Should warn, but show the same value

  // Check various declarations of a class with the new field present
  var c7: ReplaceParam = new ReplaceParam(4);
  var c8: ReplaceParam(2) = new ReplaceParam(2);
  // Check direct references
  writeln(c7.newName);
  writeln(c7.oldName); // Should warn, but show the same value
  writeln(c8.newName);
  writeln(c8.oldName); // Should warn, but show the same value

  // Check various declarations of a record with the new field present
  var r7: ReplaceParam2 = new ReplaceParam2(4);
  var r8 = new ReplaceParam2(2);
  var r9: ReplaceParam2(1);
  // Check direct references
  writeln(r7.newName);
  writeln(r7.oldName); // Should warn, but show the same value
  writeln(r8.newName);
  writeln(r8.oldName); // Should warn, but show the same value
  writeln(r9.newName);
  writeln(r9.oldName); // Should warn, but show the same value

  // Check various declarations of a class with the new field present
  var c10: ReplaceConstConcrete = new ReplaceConstConcrete(3);
  var c11 = new ReplaceConstConcrete(17);
  // Check direct references
  writeln(c10.newName);
  writeln(c10.oldName); // Should warn, but show the same value
  writeln(c11.newName);
  writeln(c11.oldName); // Should warn, but show the same value

  // Check various declarations of a record with the new field present
  var r10: ReplaceConstConcrete2 = new ReplaceConstConcrete2(3);
  var r11 = new ReplaceConstConcrete2(17);
  var r12: ReplaceConstConcrete2;
  // Check direct references
  writeln(r10.newName);
  writeln(r10.oldName); // Should warn, but show the same value
  writeln(r11.newName);
  writeln(r11.oldName); // Should warn, but show the same value
  writeln(r12.newName);
  writeln(r12.oldName); // Should warn, but show the same value

  // Check various declarations of a class with the new field present
  var c13: ReplaceVarGeneric = new ReplaceVarGeneric(10);
  var c14 = new ReplaceVarGeneric(0);
  // Check direct references
  writeln(c13.newName);
  writeln(c13.oldName); // Should warn, but show the same value
  // Check that the two "fields" stay in sync if new field is updated
  c14.newName += 3;
  writeln(c14.newName);
  writeln(c14.oldName); // Should warn, but show the same value
  // Check that the two "fields" stay in sync if old "field" is updated
  c14.oldName -= 2; // Should warn
  writeln(c14.newName);
  writeln(c14.oldName); // Should warn, but show the same value

  // Check various declarations of a record with the new field present
  var r13: ReplaceVarGeneric2 = new ReplaceVarGeneric2(10);
  var r14 = new ReplaceVarGeneric2(0);
  var r15: ReplaceVarGeneric2(int);
  // Check direct references
  writeln(r13.newName);
  writeln(r13.oldName); // Should warn, but show the same value
  // Check that the two "fields" stay in sync if new field is updated
  r14.newName += 3;
  writeln(r14.newName);
  writeln(r14.oldName); // Should warn, but show the same value
  // Check that the two "fields" stay in sync if old "field" is updated
  r15.oldName -= 2; // Should warn
  writeln(r15.newName);
  writeln(r15.oldName); // Should warn, but show the same value

  // Check various declarations of a class with the new field present
  var c16: ReplaceConstGeneric = new ReplaceConstGeneric(3);
  var c17 = new ReplaceConstGeneric(17);
  // Check direct references
  writeln(c16.newName);
  writeln(c16.oldName); // Should warn, but show the same value
  writeln(c17.newName);
  writeln(c17.oldName); // Should warn, but show the same value

  // Check various declarations of a record with the new field present
  var r16: ReplaceConstGeneric2 = new ReplaceConstGeneric2(3);
  var r17 = new ReplaceConstGeneric2(17);
  var r18: ReplaceConstGeneric2(int);
  // Check direct references
  writeln(r16.newName);
  writeln(r16.oldName); // Should warn, but show the same value
  writeln(r17.newName);
  writeln(r17.oldName); // Should warn, but show the same value
  writeln(r18.newName);
  writeln(r18.oldName); // Should warn, but show the same value
}
