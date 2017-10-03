/*
 * buf.c
 *
 *  Created on: 2016. 7. 12.
 *      Author: WONHEELEE
 */
#include "buf.h"
#include "mac.h"
#include "constants.h"
#include "string.h"


static buffer_t buf_pool[MAX_BUF_POOL_SIZE];    ///< Creates the frame buffer pool



/**************************************************************************/
/*!
    Init the buffer pool.
*/
/**************************************************************************/
void buf_init()
{
    uint8 i;

    for (i=0; i<MAX_BUF_POOL_SIZE; i++)
    {
        memset(&buf_pool[i], 0, sizeof(buffer_t));
    }
}

/**************************************************************************/
/*!
    Return the number of bufs currently allocated.
*/
/**************************************************************************/
uint8 buf_get_cnt()
{
    uint8 i;
    uint8 buf_cnt = 0;

    for (i=0; i<MAX_BUF_POOL_SIZE; i++)
    {
        if (buf_pool[i].alloc)
        {
            buf_cnt++;
        }
    }
    return buf_cnt;
}

/**************************************************************************/
/*!
    Allocate and return a pointer to a frame buffer.
*/
/**************************************************************************/
buffer_t *buf_get(uint8 tx_rx)
{
    uint8 i;
    for (i=0; i<MAX_BUF_POOL_SIZE; i++)
    {
        if (!buf_pool[i].alloc)
        {
            if (tx_rx)
            {
                buf_pool[i].dptr = &buf_pool[i].buf[aMaxPHYPacketSize];
            }
            else
            {
                buf_pool[i].dptr = &buf_pool[i].buf[0];
            }
            buf_pool[i].len = 0;
            buf_pool[i].index = i;
            buf_pool[i].alloc = 1;
            return &buf_pool[i];
        }
    }
    return NULL;
}

/**************************************************************************/
/*!
    Free a buffer that has been allocated.
*/
/**************************************************************************/
void buf_free(buffer_t *buf)
{
    if (buf)
    {
        buf->alloc = 0;
    }
}
