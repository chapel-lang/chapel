#ifndef QT_ENVARIABLES_H
#define QT_ENVARIABLES_H

#include "qt_visibility.h"

char const INTERNAL *qt_internal_get_env_str(char const *envariable,
                                             char const *dflt);
unsigned long INTERNAL qt_internal_get_env_num(char const *envariable,
                                               unsigned long dflt,
                                               unsigned long zerodflt);
unsigned char INTERNAL qt_internal_get_env_bool(char const *envariable,
                                                unsigned char dflt);
int INTERNAL qt_internal_unset_envstr(char const *envariable);
#endif // ifndef QT_ENVARIABLES_H
/* vim:set expandtab: */
