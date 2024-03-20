import sys
import os
import re
import argparse

num_replaced = 0

def replaceSingleInstance(line, from_string, to_string, files_to_update, regex):
  # Extract the file path and line number
  file_path, line_number = line.split(':')[:2]
  # Check if the line number is actually a number or a letter line nnn
  # only cast to int if it is a number
  if not line_number.isdigit():
    return
  line_number = int(line_number)
  
  # Replace the $CHPL_HOME variable in the filepath
  resolved_file_path = os.path.expandvars(file_path)
  file_name = os.path.basename(resolved_file_path)
  if files_to_update and file_name not in files_to_update:
    print("Skipping ", resolved_file_path, " because it is not in the list of files to replace")
    return
  # Check if the file exists
  # Skip if it doesn't
  if not os.path.isfile(resolved_file_path):
    print("File ", resolved_file_path," does not exist")
    return

  # Open the file and go to the specified line number
  with open(resolved_file_path, 'r') as f:
    lines = f.readlines()
    old_line = lines[line_number - 1]

  # Replace the word with another word
  if regex:
    new_line = re.sub(from_string, to_string, old_line)
  else:
    new_line = old_line.replace(from_string, to_string)
  # Check if the line was actually replaced
  # If it was, increment the number of replacements
  if old_line != new_line:
    print("Replacing ", from_string, " with ", to_string, " in ", resolved_file_path, " at line ", line_number)
    global num_replaced
    num_replaced += 1

  # Write the new line back to the file
  lines[line_number - 1] = new_line
  with open(resolved_file_path, 'w') as f:
    f.writelines(lines)


def replaceAllInstances(lines_file, from_string, to_string, files_to_update,
                        regex):
  # Read the warning lines from the file
  with open(lines_file, 'r') as f:
    lines = f.readlines()
    # Replace
    for line in lines:
      replaceSingleInstance(line, from_string, to_string, files_to_update, regex)
    
def main():
  parser = argparse.ArgumentParser()

  parser.add_argument("from_string")
  parser.add_argument("to_string")
  parser.add_argument("lines_file")
  parser.add_argument("-r", "--regex", action="store_true")

  # files_to_update: If specified, only carry on the replacement operation in
  # the specified files
  args, files_to_update = parser.parse_known_args()
  
  # Take in command line arguments
  from_string = args.from_string
  to_string = args.to_string
  lines_file = args.lines_file
  regex = args.regex
  
  # Replace all instances of deprecated with replacement
  replaceAllInstances(lines_file, from_string, to_string, files_to_update, regex)
  print("Replaced ", num_replaced, " instances of ", from_string, " with ", to_string)
if __name__ == "__main__":
  main()
