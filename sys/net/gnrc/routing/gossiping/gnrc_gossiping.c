#include <stdint.h>
#include <errno.h>

#include "net/gnrc/gossiping.h"
#include "net/gnrc.h"

#define ENABLE_DEBUG    (1)
#include "debug.h"

/**
 * @brief   Save the gossiping's thread PID for later reference
 */
static kernel_pid_t _pid = KERNEL_PID_UNDEF;

/**
 * @brief   Allocate memory for the gossiping thread's stack
 */
#if ENABLE_DEBUG
static char _stack[GNRC_GOSSIPING_STACK_SIZE + THREAD_EXTRA_STACKSIZE_PRINTF];
#else
static char _stack[GNRC_GOSSIPING_STACK_SIZE];
#endif

uint32_t hash_msg(msg_t msg)
{
    DEBUG("gossiping: first: %x\n", msg.type);
    
    gnrc_pktsnip_t* pkt = msg.content;
    DEBUG("gossiping: second: %x\n", pkt.type);
/*    switch (msg.type) {*/
/*        case GNRC_NETAPI_MSG_TYPE_RCV:*/
/*            break;*/
/*        case GNRC_NETAPI_MSG_TYPE_SND:*/
/*            break;*/
/*        case GNRC_NETAPI_MSG_TYPE_SET:*/
/*            break;*/
/*        case GNRC_NETAPI_MSG_TYPE_GET:*/
/*            break;*/
/*        default:*/
/*            break;*/
/*    }*/
    return 0;
}

void *_event_loop(void *arg)
{
    static msg_t _msg_q[GNRC_GOSSIPING_STACK_SIZE];
    (void) arg;
    
    DEBUG("gossiping: Debug enabled\n");
    
    msg_t msg, reply;
    reply.type = GNRC_NETAPI_MSG_TYPE_ACK;
    reply.content.value = -ENOTSUP;
    msg_init_queue(_msg_q, GNRC_GOSSIPING_STACK_SIZE);
/*    gnrc_pktsnip_t *pkt = NULL;*/
    struct gnrc_netreg_entry me_reg = GNRC_NETREG_ENTRY_INIT_PID(
                                 GNRC_NETREG_DEMUX_CTX_ALL,
                                 sched_active_pid);
    gnrc_netreg_register(GNRC_NETTYPE_IPV6, &me_reg);
    gnrc_netreg_register(GNRC_NETTYPE_ICMPV6, &me_reg);
    while (1) {
        msg_receive(&msg);
        switch (msg.type) {
            case GNRC_NETAPI_MSG_TYPE_RCV:
                DEBUG("gossiping: GNRC_NETAPI_MSG_TYPE_RCV\n");
                break;
            case GNRC_NETAPI_MSG_TYPE_SND:
                DEBUG("gossiping: GNRC_NETAPI_MSG_TYPE_SND\n");
                break;
             case GNRC_NETAPI_MSG_TYPE_SET:
             case GNRC_NETAPI_MSG_TYPE_GET:
                msg_reply(&msg, &reply);
                break;
            default:
                DEBUG("gossiping: received unidentified message\n");
                break;
        }
    }
    return NULL;
}

int gnrc_gossiping_init(void)
{
    /* check if thread is already running */
    if (_pid == KERNEL_PID_UNDEF) {
        /* start UDP thread */
        _pid = thread_create(_stack, sizeof(_stack), GNRC_GOSSIPING_PRIO,
                             THREAD_CREATE_STACKTEST, _event_loop, NULL, "gossiping");
    }
    return _pid;
}
