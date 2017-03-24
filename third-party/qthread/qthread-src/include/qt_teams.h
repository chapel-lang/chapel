#ifndef QT_TEAMS_H
#define QT_TEAMS_H

#include "qthread/sinc.h"
#include "qt_visibility.h"
#include "qt_qthread_t.h"

#ifdef QTHREAD_USE_EUREKAS
#include <signal.h> /* should have SIGUSR1 and SIGUSR2, per P90 */
#define QT_ASSASSINATE_SIGNAL SIGUSR1
#define QT_EUREKA_SIGNAL      SIGUSR2
#endif /* QTHREAD_USE_EUREKAS */

/* flags for teams (must be different bits) */
#define QTHREAD_TEAM_DEAD             (1 << 0)
#define QTHREAD_TEAM_RESERVED_1       (1 << 1)
#define QTHREAD_TEAM_RET_IS_SYNCVAR   (1 << 2)
#define QTHREAD_TEAM_RET_IS_SINC      (1 << 3)
#define QTHREAD_TEAM_RET_IS_VOID_SINC ((1 << 3) | (1 << 2))
#define QTHREAD_TEAM_RESERVED_4       (1 << 4)
#define QTHREAD_TEAM_RESERVED_5       (1 << 5)
#define QTHREAD_TEAM_RESERVED_6       (1 << 6)
#define QTHREAD_TEAM_RESERVED_7       (1 << 7)

#define QTHREAD_TEAM_RET_MASK (QTHREAD_TEAM_RET_IS_SYNCVAR | QTHREAD_TEAM_RET_IS_SINC)

/* internal signalling macros */
#define TEAM_SIGNAL_ISEXIT(team_id)   (((saligned_t)team_id) > 1)
#define TEAM_SIGNAL_ISEUREKA(team_id) (((saligned_t)team_id) < 0)
#define TEAM_SIGNAL_SENDERID(team_id) (team_id)
#define TEAM_SIGNAL_EXIT(team_id)     (team_id)
#define TEAM_SIGNAL_EUREKA(team_id)   (-1 * (saligned_t)(team_id))

typedef struct qt_team_s {
    aligned_t    eureka;
    aligned_t    eureka_lock;
    aligned_t   *parent_eureka;
    unsigned int team_id;
    aligned_t    watcher_started;
    qt_sinc_t   *sinc;
    qt_sinc_t   *subteams_sinc;
    unsigned int parent_id;
    qt_sinc_t   *parent_subteams_sinc;
    void        *return_loc;
    uint_fast8_t flags;
} qt_team_t;

void INTERNAL qt_internal_teams_init(void);
void INTERNAL qt_internal_teams_reclaim(void);
void INTERNAL qt_internal_teamfinish(qt_team_t   *team,
                                     uint_fast8_t flags);
qt_team_t INTERNAL *qt_internal_team_new(void *restrict      ret,
                                         unsigned int        feature_flag,
                                         qt_team_t *restrict curr_team,
                                         unsigned int        parent_id);
void INTERNAL qt_internal_subteam_leader(qthread_t *t);
#endif // ifndef QT_TEAMS_H
/* vim:set expandtab: */
