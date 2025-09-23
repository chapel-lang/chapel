def debuggerBreakHere_callback(frame, bp_loc, internal_dict):
    thread = frame.GetThread()
    thread.SetSelectedFrame(1)

    curFrame = thread.GetSelectedFrame()
    fn = curFrame.GetFunction()
    if fn and fn.name.startswith("debuggerBreakHere("):
        thread.SetSelectedFrame(2)

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand('breakpoint command delete debuggerBreakHere')
    debugger.HandleCommand('breakpoint command add --python-function chpl_lldb_debuggerBreakHere.debuggerBreakHere_callback debuggerBreakHere')
