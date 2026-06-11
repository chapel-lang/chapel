import lldb


class DebuggerBreakHereStopHook:
    def __init__(self, target, extra_args, internal_dict):
        pass

    def handle_stop(self, exe_ctx, stream):
        thread = exe_ctx.GetThread()
        if not thread or not thread.IsValid():
            return True

        # only do something if we hit the debuggerBreakHere breakpoint
        stopReason = thread.GetStopReason()
        if stopReason != lldb.eStopReasonBreakpoint:
            return True
        count = thread.GetStopReasonDataCount()
        if count < 2:
            return True
        brkId = thread.GetStopReasonDataAtIndex(0)
        breakpointInfo = (
            thread.GetProcess().GetTarget().FindBreakpointByID(brkId)
        )
        isNamedDebuggerBreakHere = (
            breakpointInfo
            and breakpointInfo.IsValid()
            and breakpointInfo.MatchesName("debuggerBreakHere")
        )
        numLocations = breakpointInfo.GetNumLocations()
        if numLocations < 1:
            return True
        location = breakpointInfo.GetLocationAtIndex(0)
        if not location or not location.IsValid():
            return True
        breakFunc = location.GetAddress().GetFunction()
        isBreakOnDebuggerBreakHere = (
            breakFunc
            and breakFunc.IsValid()
            and breakFunc.GetName() == "debuggerBreakHere"
        )

        if not (isNamedDebuggerBreakHere or isBreakOnDebuggerBreakHere):
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
