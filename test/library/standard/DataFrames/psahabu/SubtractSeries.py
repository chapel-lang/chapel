import pandas as pd

I = ["A", "B", "C", "D", "E"]

oneDigit = pd.Series([1, 2, 3, 4, 5], pd.Index(I))
twoDigit = pd.Series([11, 22, 33, 44, 55], pd.Index(I))

print "terms:"
print oneDigit
print twoDigit
print
print "difference:"
print twoDigit - oneDigit
print

I2 = ["A", "B", "C"]
I3 = ["B", "C", "D", "E"]
X = pd.Series([5, 1, 2], pd.Index(I2))
Y = pd.Series([10, 20, 6, 7], pd.Index(I3))

print "terms:"
print X
print Y
print
print "difference:"
print X - Y
