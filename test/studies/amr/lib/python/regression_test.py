import os, re, sys


#|\""""""""""""""""""""""""|\
#| >    approximateDiff    | >
#|/________________________|/

#-----------------------------------------------------------
# Takes the "approximate diff" of two files, allowing for
# differences in real data up to the specified tolerance.
#
# This isn't as sophisticated as a true diff in terms of
# matching largest common subsections.  It's just meant
# to speed the user's analysis if changing the code results
# in structural or numerical differences.
#-----------------------------------------------------------

def approximateDiff(file_name_1, file_name_2, tolerance):
  
    def approximatelyEqual(x,y,tolerance):
        if x==y:
            return True

        try:
            if abs(float(x) - float(y)) < tolerance:
                return True
        except:
            return False


    file_1 = open(file_name_1, 'r')
    file_2 = open(file_name_2, 'r')
    lines_1 = file_1.readlines()
    lines_2 = file_2.readlines()
    difference = False
    any_difference = False
    
    diff_output = []
  
    #==== Check file lengths ====
    #--------------------------------------------------------------------
    # A problem here will indicate that something is structurally wrong.
    #--------------------------------------------------------------------
    if not(len(lines_1) == len(lines_2)):
        diff_output.append("Files are of different length")


    #==== Check line by line ====
    #----------------------------------------------------------------------
    # This is where numerical differences will be highlighted.  Also, if
    # the files are comparable up to a certain point, this will show where
    # they begin to diverge.
    #----------------------------------------------------------------------
    for i in range(min(len(lines_1), len(lines_2))):
        split_1 = lines_1[i].split();
        split_2 = lines_2[i].split();
  
        if len(split_1) == len(split_2):
            #-----------------------------------------------------------
            # If lines have the same number of elements, then check for
            # numerical differences.
            #-----------------------------------------------------------
            for j in range(len(split_1)):
                if not(approximatelyEqual(split_1[j],split_2[j],tolerance)):
                    diff_output.append("Line " +  str(i+1) + ", element " + str(j+1) + ": elements differ")
                    diff_output.append("  " + file_name_1 + ": " + split_1[j])
                    diff_output.append("  " + file_name_2 + ": " + split_2[j])
        else:
            #-----------------------------------------------------------
            # If lines have a different number of elements, then print
            # their contents.
            #-----------------------------------------------------------
            diff_output.append("Line " + str(i+1) + ": number of elements differs")
            diff_output.append("  " + file_name_1 + ": " + lines_1[i])
            diff_output.append("  " + file_name_2 + ": " + lines_2[i])

    return diff_output                    
# /|""""""""""""""""""""""""/|
#< |    approximateDiff    < |
# \|________________________\|




#==== Name of the diff output file ====
#----------------------------------------------------------
# The name of this script is sys.argv[0].  The name of the
# output file should follow immediately.
#----------------------------------------------------------
output_file_name = sys.argv[1]

if output_file_name.endswith(".comp.out.tmp"):
    output_file = open(output_file_name, 'w')
    output_file.write("Compilation failed!\n");
    output_file.write("See the errors in the file "+output_file_name+".save\n")
    output_file.close()
    exit(0)

output_file = open(output_file_name, 'w')

#==== Tolerance for approximate diff-ing ====
tolerance = 1e-4


#==== Locate data files in the regression_data directory ====
regression_files = os.listdir("regression_data")
data_pattern = re.compile("fort.q\d{4}")
data_files = []

for file_name in regression_files:
    if data_pattern.match(file_name):
        data_files.append(file_name)
        
        
#==== Compare each data file to the same file in _output ====
difference_detected = False
failure = False
for data_file in data_files:
    try:
        diff_results = approximateDiff("_output/"+data_file, "regression_data/"+data_file, tolerance)
        if len(diff_results) > 0:
            for line in diff_results:
                output_file.write(line + "\n")
                difference_detected = True
    except Exception as exn:
        #---- If the diff fails, indicate the file with which it crashed ----
        output_file.write("Test failed.\n")
        output_file.write("Could not use approximateDiff with file " + data_file + ".\n")
        output_file.write('%s\n'%(exn))
        failure = True
        break

#==== Write an OK message if there were no problems ====
if not(difference_detected) and not(failure):
    output_file.write("No change detected.\n")
    

output_file.close()


