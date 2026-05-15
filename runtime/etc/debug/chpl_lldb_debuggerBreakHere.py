class DebuggerBreakHereStopHook:
    def __init__(self, target, extra_args, internal_dict):
        pass

    def handle_stop(self, exe_ctx, stream):
        thread = exe_ctx.GetThread()
        if not thread or not thread.IsValid():
            return True

        thread.SetSelectedFrame(1)

        curFrame = thread.GetSelectedFrame()
        fn = curFrame.GetFunction()
        if fn and fn.name.startswith("debuggerBreakHere("):
            thread.SetSelectedFrame(2)
        return True


def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand("target stop-hook delete 1")
    debugger.HandleCommand(
        "target stop-hook add --script-class chpl_lldb_debuggerBreakHere.DebuggerBreakHereStopHook"
    )
