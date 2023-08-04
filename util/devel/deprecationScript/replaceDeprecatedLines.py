import sys
import os

num_replaced = 0

def replaceSingleInstance(line, deprecated, replacement, include_files):
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
  if include_files and file_name not in include_files:
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
  new_line = old_line.replace(deprecated, replacement)
  # Check if the line was actually replaced
  # If it was, increment the number of replacements
  if old_line != new_line:
    print("Replacing ", deprecated, " with ", replacement, " in ", resolved_file_path, " at line ", line_number)
    global num_replaced
    num_replaced += 1

  # Write the new line back to the file
  lines[line_number - 1] = new_line
  with open(resolved_file_path, 'w') as f:
    f.writelines(lines)


def replaceAllInstances(warning_lines, deprecated, replacement, include_files):
  # Read the warning lines from the file
  with open(warning_lines, 'r') as f:
    lines = f.readlines()
    # Replace
    for line in lines:
      replaceSingleInstance(line, deprecated, replacement, include_files)
    
def main():
  # Print usage if the user does not provide the correct number of arguments
  if len(sys.argv) < 4:
    print("Usage: python3 replaceDeprecatedLines.py <log_file> <deprecated> <replacement> [...include_files]")
    exit(1)
  
  # Take in command line arguments
  warning_file = sys.argv[1]
  deprecated = sys.argv[2]
  replacement = sys.argv[3]
  # If specified, only carry on the replacement operation in the specified files
  include_files = sys.argv[4:]
  
  # Replace all instances of deprecated with replacement
  replaceAllInstances(warning_file, deprecated, replacement, include_files)
  print("Replaced ", num_replaced, " instances of ", deprecated, " with ", replacement)
if __name__ == "__main__":
  main()
