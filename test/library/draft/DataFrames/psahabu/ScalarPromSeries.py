import pandas as pd

oneDigit = pd.Series([1, 2, 3, 4, 5], pd.Index(["A", "B", "C", "D", "E"]))
n = 12

print "series:"
print oneDigit
print "scalar:"
print n

print
print "sum:"
print oneDigit + n 
print "difference:"
print oneDigit - n 
print "product:"
print oneDigit * n 
