import stringArrayArg

# note: would be nice to merge with use_retStringArray.py when this works,
# so it can test sending the returned array back like the other array tests do
stringArrayArg.chpl_setup()
x = ["let's", "see", "if", "this", "works"]
stringArrayArg.takesStrArray(x)
stringArrayArg.chpl_cleanup()
