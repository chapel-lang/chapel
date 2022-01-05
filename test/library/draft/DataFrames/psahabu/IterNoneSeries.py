import pandas as pd

I = ["A", "B", "C", "D", "E"]
A = ["a", None, "c", None, "e"]

letters = pd.Series(A, pd.Index(I))

for i in letters.index:
    print i

print
for i in zip(letters.index, range(0, 5)):
    print i 

print
for i in letters:
    print i

print
for i in letters.items():
    print i

print
print letters
