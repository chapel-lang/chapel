import pandas as pd

I = ["A", "B", "C", "D", "E"]
noNone = pd.Series([1, 2, 3, 4, 5], pd.Index(I))
someNone = pd.Series([1, None, 3, None, 5], pd.Index(I))
moreNone = pd.Series([None, 20, None, 40, None], pd.Index(I))

print "noNone:"
print noNone
print
print "someNone:"
print someNone
print
print "moreNone:"
print moreNone

print
print "noNone.sum():" + str(noNone.sum())
print
print "someNone.sum():" + str(someNone.sum())
print
print "moreNone.sum():" + str(moreNone.sum())

print
print "noNone.min():" + str(noNone.min())
print
print "someNone.min():" + str(someNone.min())
print
print "moreNone.min():" + str(moreNone.min())

print
print "noNone.max():" + str(noNone.max())
print
print "someNone.max():" + str(someNone.max())
print
print "moreNone.max():" + str(moreNone.max())
