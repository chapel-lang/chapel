import pandas as pd

I = ["A", "B", "C", "D", "E"]

oneDigit = pd.Series([1, 2, 3, 4, 5], pd.Index(I))
twoDigit = pd.Series([10, 20, 30, 40, 50], pd.Index(I))

print "addends:"
print oneDigit
print twoDigit
print
print "sum:"
print oneDigit + twoDigit
print

I2 = ["A", "B", "C"]
I3 = ["B", "C", "D", "E"]
X = pd.Series([0, 1, 2], pd.Index(I2))
Y = pd.Series([10, 20, 0, 0], pd.Index(I3))

print "addends:"
print X
print Y
print
print "sum:"
print X + Y
print

A = pd.Series(["hello ", "my ", "name", "is", "brad"])
B = pd.Series(["world", "real"])

print "addends:"
print A
print B
print 
print "sum: "
print A + B
