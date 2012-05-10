#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h> /* for getenv() and strtoul() */
#include <stdio.h>  /* for fprintf() */
#include <assert.h>
#include <string.h>

#include "qt_visibility.h"
#include "qt_envariables.h"
#include "qt_debug.h"

static int info = -1;

static const char *qt_internal_envstr(const char *envariable)
{
    char        mod_envariable[100];
    const char *str;

    snprintf(mod_envariable, 100, "QT_%s", envariable);
    str = getenv(mod_envariable);
    qthread_debug(CORE_BEHAVIOR, "checking envariable %s\n", envariable);
    if (str && *str) {
        if (info) {
            printf("%s = %s\n", mod_envariable, str);
        }
        return str;
    } else {
        snprintf(mod_envariable, 100, "QTHREAD_%s", envariable);
        str = getenv(mod_envariable);
        if (str && *str) {
            if (info) {
                printf("%s = %s\n", mod_envariable, str);
            }
            return str;
        }
    }
    if (info) {
        printf("[QT|QTHREAD]_%s = (unset)\n", envariable);
    }
    return NULL;
}

static inline void check_info(void)
{
    if (-1 == info) {
        const char *str;
        str = getenv("QT_INFO");
        if (str == NULL) {
            str = getenv("QTHREAD_INFO");
        }
        if (str != NULL) {
            int   tmp;
            char *errptr;
            tmp = strtol(str, &errptr, 0);
            if (*errptr != 0) {
                info = 0;
            } else {
                info = (tmp > 1) ? 1 : 0;
            }
        } else {
            info = 0;
        }
    }
}

const char INTERNAL *qt_internal_get_env_str(const char *envariable)
{
    assert(strlen(envariable) < 90);
    check_info();
    return qt_internal_envstr(envariable);
}

unsigned long INTERNAL qt_internal_get_env_num(const char   *envariable,
                                               unsigned long dflt,
                                               unsigned long zerodflt)
{
    const char   *str = qt_internal_get_env_str(envariable);
    unsigned long tmp = dflt;

    if (str && *str) {
        char *errptr;
        tmp = strtoul(str, &errptr, 0);
        if (*errptr != 0) {
            fprintf(stderr, "unparsable %s (%s)\n", envariable, str);
            tmp = dflt;
        }
        if (tmp == 0) {
            qthread_debug(CORE_DETAILS, "since envariable %s is 0, choosing default: %u\n", envariable, zerodflt);
            tmp = zerodflt;
        } else {
            qthread_debug(CORE_DETAILS, "envariable %s parsed as %u\n", envariable, tmp);
        }
    }
    return tmp;
}

/* vim:set expandtab: */
