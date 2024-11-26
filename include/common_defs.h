#ifndef TROULETTE_COMMON_DEFS_H
#define TROULETTE_COMMON_DEFS_H

#define QOS 1
#define TIMEOUT 10000L
#define TOPIC "GameEvents"

/* Game events */
/* Events fired by clients */
#define PLAYER_JOIN_EVENT 1
#define START_TURN_EVENT 5
#define UPDATE_STATUS_EVENT 25
/* Events fired by the server */
#define KILL_PROCESS_EVENT 100
#define PLAYER_DIED_EVENT 105

#endif
