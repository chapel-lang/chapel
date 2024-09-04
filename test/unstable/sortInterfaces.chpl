use Sort;
record R1: relativeComparator { }

record R2 { }
R2 implements keyPartComparator;

record R3 { }
R3 implements sortComparator;

record R4: keyComparator { }
