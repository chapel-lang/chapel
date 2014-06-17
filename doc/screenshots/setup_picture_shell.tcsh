#
# This is intended to be source'd by somebody using tcsh in an xterm.
# It sets the xterm up to be nice for taking screen shots.
#
set titlebar=" "
set newprompt="[%] "

# Set the window title
echo -n "\033]0;${titlebar}\007"

# Set the prompt
set prompt="${newprompt}"

clear

#
# Now you're ready to take a screen shot. From a different terminal, try:
# Get the id of the window of interest using xwininfo
# import -window id -delay 3 -frame jpg:screenshot.jpg
#
