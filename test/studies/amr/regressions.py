def approximatelyEqual(x,y,tolerance):
    if x==y:
        return True

    try:
        if abs(float(x) - float(y)) < tolerance:
            return True
    except:
        return False



def approximateDiff(file_name_1, file_name_2, tolerance):
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

import os


tolerance = 1e-4
base_path = os.getcwd()


advection_grid_dir = base_path + "/advection/grid"
os.chdir(advection_grid_dir)
os.system("make output")
file_name_1 = "_output/fort.q0020"
file_name_2 = "regression_data/fort.q0020"
advection_grid_diff = approximateDiff(file_name_1, file_name_2, tolerance)
os.system("make clean")
os.chdir(base_path)



advection_level_dir = base_path + "/advection/level"
os.chdir(advection_level_dir)
os.system("make output")
file_name_1 = "_output/fort.q0020"
file_name_2 = "regression_data/fort.q0020"
advection_level_diff = approximateDiff(file_name_1, file_name_2, tolerance)
os.system("make clean")
os.chdir(base_path)


diffusion_grid_dir = base_path + "/diffusion/grid"
os.chdir(diffusion_grid_dir)
os.system("make output")
file_name_1 = "_output/fort.q0020"
file_name_2 = "regression_data/fort.q0020"
diffusion_grid_diff = approximateDiff(file_name_1, file_name_2, tolerance)
os.system("make clean")
os.chdir(base_path)


diffusion_level_dir = base_path + "/diffusion/level"
os.chdir(diffusion_level_dir)
os.system("make output")
file_name_1 = "_output/fort.q0025"
file_name_2 = "regression_data/fort.q0025"
diffusion_level_diff = approximateDiff(file_name_1, file_name_2, tolerance)
os.system("make clean")
os.chdir(base_path)


if advection_grid_diff:
    print "Warning: Change in advection/grid"
else:
    print "No change in advection/grid"

if advection_level_diff:
    print "Warning: Change in advection/level"
else:
    print "No change in advection/level"

if diffusion_grid_diff:
    print "Warning: Change in diffusion/grid"
else:
    print "No change in diffusion/grid"
    
if diffusion_level_diff:
    print "Warning: Change in diffusion/level"
else:
    print "No change in diffusion/level"