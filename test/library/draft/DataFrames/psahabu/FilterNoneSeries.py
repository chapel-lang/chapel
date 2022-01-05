import pandas as pd

I = ["A", "B", "C", "D", "E"]
oneDigit = pd.Series([1, None, 3, None, 5], pd.Index(I))
twoDigit = pd.Series([10, None, 30, None, 50], pd.Index(I))

print "oneDigit:"
print oneDigit
print
print "twoDigit:"
print twoDigit
print
print "oneDigit < 3:"
print oneDigit[oneDigit < 3]
print
print "twoDigit > 30:"
print twoDigit[twoDigit > 30]
