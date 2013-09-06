#ifndef QT_SPAWN_MACROS_H
#define QT_SPAWN_MACROS_H

#define qthread_fork_track_syncvar_copyargs(f, arg, arg_size, ret)          qthread_spawn((f),         \
                                                                                          (arg),       \
                                                                                          (arg_size),  \
                                                                                          (ret),       \
                                                                                          0,           \
                                                                                          NULL,        \
                                                                                          NO_SHEPHERD, \
                                                                                          QTHREAD_SPAWN_PARENT | QTHREAD_SPAWN_RET_SYNCVAR_T)
#define qthread_fork_track_syncvar_copyargs_to(f, arg, arg_size, ret, shep) qthread_spawn((f),        \
                                                                                          (arg),      \
                                                                                          (arg_size), \
                                                                                          (ret),      \
                                                                                          0,          \
                                                                                          NULL,       \
                                                                                          (shep),     \
                                                                                          QTHREAD_SPAWN_PARENT | QTHREAD_SPAWN_RET_SYNCVAR_T)

#endif // ifndef QT_SPAWN_MACROS_H
/* vim:set expandtab: */
