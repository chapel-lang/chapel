import os


def notestCheck(dir):
    directory_list = os.listdir(dir)
    contains_chpl   = False
    contains_notest = False

    for name in directory_list:
        full_path = dir + '/' + name
        if os.path.isdir(full_path) and name[0] != '.':
            notestCheck(full_path)
        elif name[-5:] == '.chpl':
            contains_chpl = True
        elif name == 'NOTEST':
            contains_notest = True

    if contains_chpl and not(contains_notest):
        print dir
        print "   WARNING: Contains .chpl files, but no NOTEST."
#     else:
#         print dir
#         print "   OK."


base_dir = os.getcwd()
notestCheck(base_dir)
