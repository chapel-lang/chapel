import pandas as pd

I = ["A", "B", "C", "D", "E"]
initial = pd.Series([1, 2, 3, 4, 5], pd.Index(I))
print "initial:"
print initial
print

def filterTest(title, filter1, filter2):
    assert str(filter1) == str(filter2)
    print title + ":"
    print filter1
    print "filtered(" + title + "):"
    print initial[filter1]
    print

filterTest("less than 3", initial < 3, 3 > initial)
filterTest("greater than 3", initial > 3, 3 < initial)
filterTest("equal to 3", initial == 3, 3 == initial)
filterTest("less than or equal to 3", initial <= 3, 3 >= initial)
filterTest("greater than or equal to 3", initial >= 3, 3 <= initial)
