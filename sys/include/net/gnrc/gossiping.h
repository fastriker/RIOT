#ifndef NET_GNRC_GOSSIPING_H
#define NET_GNRC_GOSSIPING_H

#include <stdint.h>

#include "byteorder.h"
#include "net/gnrc.h"
#include "net/udp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Priority of the UDP thread
 */
#ifndef GNRC_GOSSIPING_PRIO
#define GNRC_GOSSIPING_PRIO           (THREAD_PRIORITY_MAIN - 2)
#endif

/**
 * @brief   Default stack size to use for the UDP thread
 */
#ifndef GNRC_GOSSIPING_STACK_SIZE
#define GNRC_GOSSIPING_STACK_SIZE     (THREAD_STACKSIZE_DEFAULT)
#endif

// bla

/**
 * @brief   Initialize and start UDP
 *
 * @return  PID of the UDP thread
 * @return  negative value on error
 */
int gnrc_gossiping_init(void);

#ifdef __cplusplus
}
#endif

#endif /* NET_GNRC_UDP_H */
/** @} */
