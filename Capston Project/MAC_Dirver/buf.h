#include "mac.h"
#include "constants.h"
#include "string.h"


#ifndef MAC_DIRVER_BUF_H_
#define MAC_DIRVER_BUF_H_
#define MAX_BUF_POOL_SIZE 6             ///< Define the number of frame buffers here

#define BUF_ALLOC(name, txrx)                                   \
            do                                                  \
            {                                                   \
                if ((name = buf_get(txrx)) == NULL)             \
                {                                               \
                                                                                    \
                    while (1);                                  \
                }                                               \
            } while (0);

buffer_t *buf_get(uint8 tx_rx);
void buf_free(buffer_t *buf);
#endif /* MAC_DIRVER_BUF_H_ */
