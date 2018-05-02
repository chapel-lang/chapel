import pandas as pd

I = ["A", "B", "C", "D", "E"]
A = ["a", "b", "c", "d", "e"]
letters = pd.Series(A, pd.Index(I))

print letters
print
print "contains A: " + str(letters.index.contains("A"))
print "contains F: " + str(letters.index.contains("F"))
