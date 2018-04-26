import pandas as pd

I = ["A", "B", "C", "D", "E"]

oneDigit = pd.Series([1, 2, 3, 4, 5], pd.Index(I))

print "addends:"
print oneDigit > 3
print oneDigit[oneDigit > 3]
