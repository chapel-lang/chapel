#ifndef _chpl_comm_launch_h
#define _chpl_comm_launch_h

//
// This is an optional comm layer function for the launcher to call
// right before launching the user program.  It only does something if
// there is a layer-specific chpl-comm-launch.h that #defines this.
//
#ifndef CHPL_COMM_PRELAUNCH
#define CHPL_COMM_PRELAUNCH()
#endif

#endif
