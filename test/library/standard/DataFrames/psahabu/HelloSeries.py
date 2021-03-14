import pandas as pd

I = ["A", "B", "C", "D", "E"]
A = ["a", "b", "c", "d", "e"]

just_letters = pd.Series(A)
letters = pd.Series(A, pd.Index(I))

print just_letters
print letters
