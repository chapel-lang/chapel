import pandas as pd

columnOne = ["a", None, "c", None, "e"]
columnTwo = [1, None, 3, None, 5]
columnThree = [10.0, 20.0, 30.0, 40.0, 50.0]

idx = pd.Index(["rowOne", "rowTwo", "rowThree", "rowFour", "rowFive"])
dataFrame = pd.DataFrame({ "columnOne": columnOne,
                           "columnTwo": columnTwo,
                           "columnThree": columnThree },
                          idx)
noIndex = pd.DataFrame({ "columnOne": columnOne,
                         "columnTwo": columnTwo,
                         "columnThree": columnThree })


print dataFrame
print
print dataFrame["columnThree"]
print
print noIndex
