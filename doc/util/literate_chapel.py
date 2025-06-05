#!/usr/bin/env python3
# encoding: utf-8

from __future__ import print_function

import sys
import re

def title_comment(line):
    """Condition for a line to be a title comment"""
    return line.startswith('//') and len(line.lstrip('//').strip()) > 0

def metadata_comment(line):
    """Condition for a line to be a front matter comment"""
    return line.startswith('//') and re.match(r"^\/\/ [a-zA-Z]+:.*$", line)

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
    inFrontMatter = True
    foundCommentExample = False

    # Each line is prose or code-block
    for (i, line) in enumerate([l.strip('\n') for l in handle.readlines()]):
        # Skip title comment if present
        if i == 0 and title_comment(line):
            switch_type('title')
            push_line(line.lstrip('//').strip())
            switch_type('prose')
            continue

        # Collect metadata strings
        if inFrontMatter and metadata_comment(line):
            switch_type('frontmatter')
            push_line(line.lstrip('//').strip())
            continue

        inFrontMatter = False

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
            if commentstarts > 0:
                # reset indentation at the start of a block comment
                #indentation = -1
                pass
        elif line.startswith('//'):
            state = 'linecomment'
        elif 'code' in laststate:
            state = 'code'
        else:
            state = 'codeblock'

        if 'comment' in state:
            proseline = line
            if state == 'linecomment':
                # Strip white space for line comments
                proseline = proseline.replace('//', '  ', 1)
                proseline = proseline.strip()
                indentation = -1 # indentation should be recomputed for /* */
            else:
                # Preserve white space for block comments, for indent purposes
                if commentstarts:
                    proseline = proseline.replace('/*', '  ')
                if commentends > 0:
                    proseline = proseline.replace('*/', '  ')
                # No need for trailing white space... ever
                proseline = proseline.rstrip(' ')

                # Handle indentation for block comments
                if indentation == -1 and proseline.strip() != "":
                    # Detect level of indentation (count leading whitespaces)
                    baseline = len(proseline) - len(proseline.lstrip(' '))
                    if baseline > 0:
                        # Set indentation for this block
                        indentation = baseline
                        # Set indentation for baseline to 0
                        proseline = proseline.lstrip(' ')
                    elif baseline == 0:
                        indentation = 0
                elif indentation > 0:
                    # Remove the the leading whitespace according
                    # to the indent level computed previously for this block
                    if proseline.startswith(' '*indentation):
                        proseline = proseline[indentation:]
                    else:
                        proseline = proseline.lstrip(' ')

            # Special case for multi line comment in learnChapelInYMinutes
            if islearnChapelInYMinutes and 'multi-line comment' in proseline:
                push_line(' '*indentation + '/*')
                push_line(proseline)
                push_line(' '*indentation + '*/')
                if foundCommentExample:
                    print("Error: Found more than one line containing \"multi-line comment\" in \
                        learnChapelInYMinutes.chpl")
                    sys.exit(1)
                foundCommentExample = True
            else:
               switch_type('prose')
               push_line(proseline)
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
        # output.append(('output', chunk_index))

        # Don't print the last chunk, it's usually empty anyway.
        # If a user wants the "last output", they can just write another
        # __BREAK__.
        pass

    if islearnChapelInYMinutes and not foundCommentExample:
        print('Error: Failed to find special case of comment example in learnChapelInYMinutes.chpl')
        sys.exit(1)
    return output
