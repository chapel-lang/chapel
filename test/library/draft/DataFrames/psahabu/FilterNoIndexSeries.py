import pandas as pd

noIndex = pd.Series([1, 2, 3, 4, 5])
print "noIndex:"
print noIndex
print

def filterTest(title, filter1, filter2):
    assert str(filter1) == str(filter2)
    print title + ":"
    print filter1
    print "filtered(" + title + "):"
    print noIndex[filter1]
    print

filterTest("less than 3", noIndex < 3, 3 > noIndex)
filterTest("greater than 3", noIndex > 3, 3 < noIndex)
filterTest("equal to 3", noIndex == 3, 3 == noIndex)
filterTest("less than or equal to 3", noIndex <= 3, 3 >= noIndex)
filterTest("greater than or equal to 3", noIndex >= 3, 3 <= noIndex)
