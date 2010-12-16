import os, re



#|\""""""""""""""""""""""""|\
#| >    approximateDiff    | >
#|/________________________|/

#---------------------------------------------------------
# Takes the "approximate diff" of two files, allowing for
# differences in real data up to the specified tolerance.
#---------------------------------------------------------

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
  
    if not(len(lines_1) == len(lines_2)):
        print "Files are of different length"
        difference = True
        any_difference = True
    
    if not(difference):
        for i in range(len(lines_1)):
            s1 = lines_1[i].split();
            s2 = lines_2[i].split();
            difference = False
      
            #==== Compare line lengths ====
            if not(len(s1) == len(s2)):
                print "Line ", i, ": number of elements differs"
                print file_name_1, ": ", lines_1[i]
                print file_name_2, ": ", lines_2[i]
                difference = True
                any_difference = True
       
            #==== Compare individual elements ====
            if not(difference):
                for j in range(len(s1)):
                    difference = not(approximatelyEqual(s1[j],s2[j],tolerance))
            
                    if difference:
                        print "Line ", i, ", element ", j, ": elements differ"
                        print file_name_1, ": ", s1[j]
                        print file_name_2, ": ", s2[j]

    return any_difference                    
# /|""""""""""""""""""""""""/|
#< |    approximateDiff    < |
# \|________________________\|





tolerance = 1e-4


#---- Locate data files in the regression_data directory ----
regression_files = os.listdir("regression_data")
data_pattern = re.compile("fort.q\d{4}")
data_files = []

for file_name in regression_files:
    if data_pattern.match(file_name):
        data_files.append(file_name)
        
        
#---- Compare each data file to the same file in _output ----
difference = False
failure = False
for data_file in data_files:
    try:
        difference = difference or approximateDiff("_output/"+data_file, "regression_data/"+data_file, tolerance)
    except:
        failure = True



#---- Write outcome to status file ----
status_file = open("regression_status.txt", "w")

if failure:
    status_file.write("Warning: Test failed.\n")
else:
    if difference:
        status_file.write("Warning: Change in data.\n")
    else:
        status_file.write("No change in data.\n")

status_file.close()


