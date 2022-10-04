#!/usr/bin/env python3
# encoding: utf-8

from __future__ import print_function

import sys

def titlecomment(line):
    """Condition for a line to be a title comment"""
    return line.startswith('//') and len(line.lstrip('//').strip()) > 0

def to_pieces(handle, islearnChapelInYMinutes=False):
    output = []
    current_type = ''
    current_content = []

    def flush():
        nonlocal current_content, current_type
        if len(current_content) != 0 and any(s != '' for s in current_content):
            while current_content[0] == '': current_content.pop(0)
            while current_content[-1] == '': current_content.pop(-1)
            output.append((current_type, current_content))
            current_content = []


    def switch_type(new_type):
        nonlocal current_type
        if current_type == new_type:
            return
        flush()
        current_type = new_type

    def push_line(line):
        nonlocal current_content
        current_content.append(line)

    commentdepth = 0
    state = ''
    indentation = -1
    chunk_index = 0
    foundCommentExample = False

    # Each line is rst or code-block
    for (i, line) in enumerate([l.strip('\n') for l in handle.readlines()]):
        # Skip title comment if present
        if i == 0 and titlecomment(line):
            switch_type('title')
            push_line(line.lstrip('//').strip())
            switch_type('prose')
            continue

        # Skip empty lines
        if len(line.strip()) == 0:
            push_line('')
            continue

        # Comment canaries - note: we don't support escaped comments: \/*
        commentstarts = line.count('/*')
        commentends = line.count('*/')
        commentdepth += commentstarts - commentends

        # State tracking
        laststate = state
        state = ''

        # Identification of line
        if commentdepth > 0 or commentends > 0:
            state = 'blockcomment'
        elif line.startswith('//'):
            state = 'linecomment'
        elif 'code' in laststate:
            state = 'code'
        else:
            state = 'codeblock'

        if 'comment' in state:
            rstline = line
            if state == 'linecomment':
                # Strip white space for line comments
                rstline = rstline.replace('//', '  ', 1)
                rstline = rstline.strip()
            else:
                # Preserve white space for block comments, for indent purposes
                if commentstarts:
                    rstline = rstline.replace('/*', '  ')
                if commentends > 0:
                    rstline = rstline.replace('*/', '  ')
                # No need for trailing white space... ever
                rstline = rstline.rstrip(' ')

            # Handle indentation
            if indentation == -1:
                # Detect level of indentation (number of leading whitespaces)
                baseline = len(rstline) - len(rstline.lstrip(' '))
                if baseline > 0:
                    # Set indentation for the proceeding block
                    indentation = baseline
                    # Set indentation for baseline to 0
                    rstline = rstline.lstrip(' ')
            else:
                # Remove the amount of indent that was removed from baseline
                if rstline.startswith(' '*indentation):
                    rstline = rstline[indentation:]
                else:
                    rstline = rstline.lstrip(' ')
            # Special case for multi line comment in learnChapelInYMinutes
            if islearnChapelInYMinutes and 'multi-line comment' in rstline:
                push_line(' '*indentation + '/*')
                push_line(rstline)
                push_line(' '*indentation + '*/')
                if foundCommentExample:
                    print("Error: Found more than one line containing \"multi-line comment\" in \
                        learnChapelInYMinutes.chpl")
                    sys.exit(1)
                foundCommentExample = True
            else:
               switch_type('prose')
               push_line(rstline)
        else:
            # Reset indentation as we enter codeblock state
            indentation = -1
            switch_type('code')

            if "__BREAK__" in line:
                flush()
                output.append(('output', chunk_index))
                chunk_index += 1
                continue

            push_line(line)

    flush()
    if chunk_index != 0:
        # We found __BREAK__; print the last chunk.
        output.append(('output', chunk_index))

    if islearnChapelInYMinutes and not foundCommentExample:
        print('Error: Failed to find special case of comment example in learnChapelInYMinutes.chpl')
        sys.exit(1)
    return output
