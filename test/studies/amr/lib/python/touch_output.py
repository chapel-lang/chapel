import time, os, re

output_directory_contents = os.listdir('_output')

timefile_pattern = re.compile('fort\.t\d{4}')

timefile_list = []

for name in output_directory_contents:
  if timefile_pattern.match(name):
    timefile_list.append(name)
  
timefile_list.sort()

for name in timefile_list:
  cmd = 'touch _output/' + name
  os.system(cmd)
  print cmd
  
  cmd = 'touch _output/fort.q' + name[-4:]
  os.system(cmd)
  print cmd
  
  time.sleep(.9)