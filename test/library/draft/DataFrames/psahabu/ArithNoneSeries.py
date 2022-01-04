import pandas as pd

I = ["A", "B", "C", "D", "E"]
oneDigit = pd.Series([1, None, 3, None, 5], pd.Index(I))
twoDigit = pd.Series([10, None, 30, None, 50], pd.Index(I))
twoDigitInv = pd.Series([None, 20, None, 40, None], pd.Index(I))

print "oneDigit:"
print oneDigit
print
print "twoDigit:"
print twoDigit
print
print "twoDigitInv:"
print twoDigitInv
print
print "oneDigit + twoDigit:"
print oneDigit + twoDigit
print
print "oneDigit - twoDigit:"
print oneDigit - twoDigit
print
print "oneDigit + twoDigitInv:"
print oneDigit + twoDigitInv
print
print "oneDigit - twoDigitInv:"
print oneDigit - twoDigitInv
