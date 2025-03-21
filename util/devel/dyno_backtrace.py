"""
Dyno stack trace script.

Though Dyno often performs re-computation, it's typical for the
compiler stacktrace to mirror (with a lot of function calls) the trace of
resolving the Chapel program. We can clean up this information, skipping
a lot of in-between frames, and print a Chapel-only trace. That's what this
script does.

To use the script, add the following lines to your .lldbinit file:

    command script import /path/to/dyno_backtrace.py
    command script add -f dyno_backtrace.backtrace dbt

Then, in the debugger, you can run the `dbt` command to print the Chapel
stack trace.
"""

import os

def get_child_by_name(value, name):
    return next(child for child in value.children if child.name == name)

def std_string_to_str(value):
    return value.GetSummary().strip('"')

def unique_string_to_str(value):
    return std_string_to_str(value.EvaluateExpression("this.str()"))

def unpack_location(root_dir, loc_value):
    file = unique_string_to_str(get_child_by_name(loc_value, "path_"))
    line = int(get_child_by_name(loc_value, "firstLine_").GetValueAsSigned())
    if file.startswith(root_dir):
        file = file[len(root_dir):]
    return file, line

def backtrace(debugger, command, result, internal_dict):
    target = debugger.GetSelectedTarget()
    root_dir = os.getcwd()
    root = target.GetProcess().GetThreadAtIndex(0).GetFrameAtIndex(0)
    frame = root

    trace = []
    while frame is not None:
        name = frame.GetFunctionName()
        if name is None:
            break

        if name.startswith("chpl::resolution::resolveFnCall("):
            rc = frame.variables[0]
            context = rc.deref.EvaluateExpression("this.context()")
            ast = frame.variables[1]

            location = frame.EvaluateExpression("parsing::locateAst({}, {})".format(context.GetName(), ast.GetName()))

            call_info = frame.variables[3]
            call_name = unique_string_to_str(get_child_by_name(call_info.deref, "name_"))
            trace.append((call_name, *unpack_location(root_dir, location), frame.idx))

        frame = frame.parent

    for i, (name, file, line, idx) in enumerate(trace):
        print("{:2d}: {} at {}:{} (lldb frame {})".format(i, name, file, line, idx))
