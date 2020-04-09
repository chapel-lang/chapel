import pandas as pd

I = ["A", "B", "C", "D", "E"]
A = ["a", "b", "c", "d", "e"]

letters = pd.Series(A, pd.Index(I))

print "A: " + letters.get("A")
print "C: " + letters.get("C")
print "E: " + letters.get("E")
print
print "1: " + letters.iat[0]
print "3: " + letters.iat[2]
print "5: " + letters.iat[4]
