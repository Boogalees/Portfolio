/*
 * mac.h
 *
 *  Created on: 2016. 7. 11.
 *      Author: WONHEELEE
 */



#ifndef MAC_LAYER_MAC_H_
#define MAC_LAYER_MAC_H_

#include "hal_types.h"


/// Calculate amount of time to perform a scan
#define MAC_SCAN_TIME(duration) (((aBaseSuperframeDuration << duration) + aBaseSuperframeDuration) >> 6)

// this define is just used to make the code more comprehensible. otherwise,
// you'd have to stare at these monsters all over the place.
#define INDIR_ENTRY(m) ((mac_indir_t *)MMEM_PTR(&m->mmem_ptr))  ///< De-reference the mem ptr and cast it as an indirect entry
#define RETRY_ENTRY(m) ((mac_retry_t *)MMEM_PTR(&m->mmem_ptr))  ///< De-reference the mem ptr and cast it as an retry entry
#define SCAN_ENTRY(m)  ((pan_descr_t *)MMEM_PTR(&m->mmem_ptr))  ///< De-reference the mem ptr and cast it as a scan entry
                                                                ///

#define MAX_SEND_BEACON_REQEST 500
#define Active_SCAN_Complete 1

#define FFD 1
#define RFD 0


struct mmem {
  struct mmem *next;
  unsigned int size;
  void *ptr;
};


typedef struct _mem_ptr_t
{
    struct _mem_ptr_t *next;    ///< Next pointer in memory
    uint8    alloc;              ///< Allocated
    struct  mmem mmem_ptr;      ///< The actual pointer to the pointer to the mem block
} mem_ptr_t;






/****************************************************************/
/*!
    Enumerated definitions for the MAC Frame Control Field in the MAC Header.
*/
/****************************************************************/
typedef enum
{
    MAC_FRM_PEND_OFF                    = 4,        ///< Data pending flag offset in FCF
    MAC_ACK_REQ_OFF                     = 5,        ///< Ack request flag offset in FCF
    MAC_PAN_ID_COMPR_OFF                = 6,        ///< Pan ID compression offset in FCF
    MAC_DEST_ADDR_MODE_OFF              = 10,       ///< Destination address mode offset in FCF
    MAC_SRC_ADDR_MODE_OFF               = 14,       ///< Source address offset in FCF

    // MAC frame control field bit masks
    MAC_FRAME_TYPE_MASK                 = 0x0003,   ///< Data, command, ACK, or beacon frame indicator
    MAC_PENDING_FRAME_MASK              = 0x0010,   ///< Indirect data awaiting retrieval for dest address
    MAC_ACK_REQ_MASK                    = 0x0020,   ///< Ack required for this frame
    MAC_PAN_ID_COMPR_MASK               = 0x0040,   ///< If src and dest PAN ID are same, then include once and set this flag
    MAC_DEST_ADDR_MODE_MASK             = 0x0C00,   ///< Address mode in address field for dest - network or extended addr
    MAC_SRC_ADDR_MODE_MASK              = 0xC000,   ///< Address mode in address field for src - network or extended addr

    // frame type for FCF enums
    MAC_BEACON                          = 0x0,      ///< Beacon frame indicator
    MAC_DATA                            = 0x1,      ///< Data frame indicator
    MAC_ACK                             = 0x2,      ///< ACK frame indicator
    MAC_COMMAND                         = 0x3,      ///< Command frame indicator

    // address mode enums
    NO_PAN_ID_ADDR                      = 0x0,      ///< No address and no PAN ID in address field
    SHORT_ADDR                          = 0x2,      ///< Address field contains network (short) address
    LONG_ADDR                           = 0x3       ///< Address field contains IEEE (long) address
} mac_frm_ctrl_enums_t;

/****************************************************************/
/*!
    Enumerated definitions for the MAC Beacon frames.
*/
/****************************************************************/
typedef enum
{
    // mac beacon fields
    MAC_GTS_MASK                        = 0x7,      ///< Guaranteed timeslot mask for beacon frame
    MAC_GTS_DESC_SIZE                   = 0x3,      ///< Guaranteed timeslot descriptor size for beacon frame
    MAC_GTS_DIR_SIZE                    = 0x1,      ///< Guaranteed timeslot direction size for beacon frame
    MAC_PENDING_SHORT_ADDR_MASK         = 0x7,      ///< Pending short address mask for beacon frame
    MAC_PENDING_LONG_ADDR_OFF           = 0x4,      ///< Pending long address offset for beacon frame
    MAC_PENDING_LONG_ADDR_MASK          = 0x70,     ///< Pending long address mask for beacon frame
    MAC_BEACON_PAYLOAD_SIZE             = 0x4,      ///< Beacon frame payload size

    // mac superframe spec fields
    MAC_ASSOC_PERMIT_OFF                = 15,
    MAC_COORDINATOR_OFF                 = 14,
    MAC_FINAL_CAP_SLOT_OFF              = 8,
    MAC_SUPERFRAME_ORDER_OFF            = 4,
    MAC_BEACON_ORDER_OFF                = 0,
    MAC_ASSOC_PERMIT_MASK               = (0x1 << MAC_ASSOC_PERMIT_OFF),    ///< Indicate if association is permitted (superfrm spec)
    MAC_COORDINATOR_MASK                = (0x1 << MAC_COORDINATOR_OFF),     ///< Indicate if coordinator (superfrm spec)
    MAC_BEACON_ORDER                    = (0xF << MAC_BEACON_ORDER_OFF),    ///< Indicate beacon order (superfrm spec)
    MAC_SUPERFRAME_ORDER                = (0xF << MAC_SUPERFRAME_ORDER_OFF), ///< Indicate superfrm order (superfrm spec)
    MAC_FINAL_CAP_SLOT                  = (0xF << MAC_FINAL_CAP_SLOT_OFF)   ///< Indicate final contention access period slot (superfrm spec)
} mac_beacon_enums_t;

/****************************************************************/
/*!
    Enumerated definitions for the MAC command frames.
*/
/****************************************************************/
typedef enum
{
    // mac command frames
    MAC_ASSOC_REQ                       = 0x1,  ///< Association request command ID
    MAC_ASSOC_RESP                      = 0x2,  ///< Association response command ID
    MAC_DATA_REQ                        = 0x4,  ///< Data request command ID
    MAC_ORPHAN_NOT                      = 0x6,  ///< Orphan notification command ID
    MAC_BEACON_REQ                      = 0x7,  ///< Beacon request command ID
    MAC_COORD_REALIGN                   = 0x8,  ///< Coordinator realignment command ID

    MAC_ASSOC_REQ_LEN                   = 0x2,
    MAC_ASSOC_RESP_LEN                  = 0x4,
    MAC_COORD_REALIGN_LEN               = 0x8
} mac_cmd_frm_enums_t;

/****************************************************************/
/*!
    Enumerated definitions for the MAC status values.
*/
/****************************************************************/
typedef enum
{
    // mac status
    MAC_SUCCESS                     = 0x0,      ///< Transaction was successful
    MAC_BEACON_LOSS                 = 0xE0,     ///< Beacon was lost (used in beacon'd networks)
    MAC_CHANNEL_ACCESS_FAILURE      = 0xE1,     ///< Unable to transmit due to channel being busy
    MAC_COUNTER_ERROR               = 0xDB,     ///< Frame counter of received frame is invalid
    MAC_DENIED                      = 0xE2,     ///< GTS request denied
    MAC_DISABLE_TRX_FAILURE         = 0xE3,     ///< Failed to disable the transceiver
    MAC_SECURITY_ERROR              = 0xE4,     ///< Frame failed decryption
    MAC_FRAME_TOO_LONG              = 0xE5,     ///< Frame exceeded maximum size
    MAC_IMPROPER_KEY_TYPE           = 0xDC,     ///< Key not allowed to be used with this frame type
    MAC_IMPROPER_SECURITY_LEVEL     = 0xDD,     ///< Frame does not meet min security level expected
    MAC_INVALID_ADDRESS             = 0xF5,     ///< Data request failed because no src or dest address
    MAC_INVALID_GTS                 = 0xE6,     ///< Invalid timeslot requested (beacon'd networks)
    MAC_INVALID_HANDLE              = 0xE7,     ///< Invalid frame data handle
    MAC_INVALID_INDEX               = 0xF9,     ///< Invalid index when trying to write MAC PIB
    MAC_INVALID_PARAMETER           = 0xE8,     ///< Invalid parameter passed to service
    MAC_LIMIT_REACHED               = 0xFA,     ///< Scan terminated because max pan descriptors reached
    MAC_NO_ACK                      = 0xE9,     ///< ACK not received after tx with ack_req flag set
    MAC_NO_BEACON                   = 0xEA,     ///< Beacon not returned after beacon request
    MAC_NO_DATA                     = 0xEB,     ///< Data frame not returned after data request (indirect poll)
    MAC_NO_SHORT_ADDRESS            = 0xEC,     ///< No short address allocated to this device (due to lack of address space)
    MAC_ON_TIME_TOO_LONG            = 0xF6,     ///< Rx enable request failed. Spec'd number of symbols longer than beacon interval
    MAC_OUT_OF_CAP                  = 0xED,     ///< Association failed due to lack of capacity (no nbor tbl entry or no address)
    MAC_PAN_ID_CONFLICT             = 0xEE,     ///< Different networks within listening range have identical PAN IDs
    MAC_PAST_TIME                   = 0xF7,     ///< Rx enable failed. Too late for current superframe and unable to be deferred
    MAC_READ_ONLY                   = 0xFB,     ///< PIB attribute is read only
    MAC_REALIGNMENT                 = 0xEF,     ///< Coordinator realignment received
    MAC_SCAN_IN_PROGRESS            = 0xFC,     ///< Request to perform scan failed because scan already in progress
    MAC_SUPERFRAME_OVERLAP          = 0xFD,     ///< Start time of beacon overlapped transmission time of coordinator beacon
    MAC_TRACKING_OFF                = 0xF8,     ///< Device not tracking beacons but instructed to send beacons based on tracked beacons
    MAC_TRANSACTION_EXPIRED         = 0xF0,     ///< Frame buffered in indirect queue expired
    MAC_TRANSACTION_OVERFLOW        = 0xF1,     ///< Exceeded maximum amount of entries in indirect queue
    MAC_TX_ACTIVE                   = 0xF2,     ///< Transmission in progress
    MAC_UNAVAILABLE_KEY             = 0xF3,     ///< Security key unavailable
    MAC_UNSUPPORTED_ATTRIBUTE       = 0xF4,     ///< Requested PIB attribute is not supported
    MAC_UNSPECIFIED_FAILURE         = 0xF5,     ///< Unspecified failure occurs. This is catch all for any type of general failure.
    MAC_UNSUPPORTED_LEGACY          = 0xDE,     ///< 802.15.4 2003 security on frame, but not supported by device
    MAC_UNSUPPORTED_SECURITY        = 0xDF      ///< Security on received frame is not supported
} mac_status_enums_t;

/****************************************************************/
/*!
    Miscellaneous enumerated definitions used by the MAC layer
*/
/****************************************************************/
typedef enum
{
    // tx options
    MAC_ACK_REQUEST                     = 0x01,     ///< Ack is required for this transmission. MAC data request service.
    MAC_INDIRECT_TRANS                  = 0x04,     ///< Frame will be sent indirectly. MAC data request service.

    // broadcast address
    MAC_BROADCAST_ADDR                  = 0xFFFF,   ///< MAC broadcast address

    // 802.15.4 Frame Versions
    MAC_802_15_4_2003                   = 0x0,      ///< Compliant to 802.15.4 2003 frame format
    MAC_802_15_4_2006                   = 0x1,      ///< Compliant to 802.15.4 2006 frame format

    // scan types
    MAC_ENERGY_SCAN                     = 0x00,     ///< Energy scan identifier
    MAC_ACTIVE_SCAN                     = 0x01,     ///< Active scan identifier
    MAC_ORPHAN_SCAN                     = 0x03,     ///< Orphan scan identifier

    MAC_MAX_CHANNELS                    = 16,       ///< Max number of channels supported by MAC
    MAC_PHY_CHANNEL_OFFSET              = 11,       ///< Channel offset. 802.15.4 range is Ch 11-26
    MAC_FCS_LENGTH                      = 2,        ///< Size of the MAC frame checksum

    // mac management states
    MLME_IDLE                           = 0x0,      ///< Idle state for the MAC management state machine
    MLME_ASSOC_REQ                      = 0x1,      ///< Association request state of MAC management state machine
    MLME_DATA_REQ                       = 0x2,      ///< Data request state of MAC management state machine
    MLME_SCAN                           = 0x3       ///< Scan state of MAC management state machine
} mac_misc_enums_t;

/*******************************************************************/
/*!
    This is the MAC Information Base. It holds all of the config
    info for the MAC layer.
*/
/*******************************************************************/
typedef struct
{
    uint16         ack_wait_duration;      ///< Time to wait for ACK from a tx'd frame
    uint8        assoc_permit;           ///< Association is allowed
    address_t   coord_addr;             ///< Address of parent this device is joined to
    uint8          dsn;                    ///< Current data sequence number
    uint8          min_be;                 ///< Minimum backoff interval exponent (interval ~ 2^min_be)
    uint8          max_csma_backoffs;      ///< Max number of CSMA backoffs before the tx is failed
    uint16         pan_id;                 ///< PAN ID that this device is joined to
    uint8        rx_on_when_idle;        ///< Receiver is always on
    uint16         resp_wait_time;         ///< Time to wait before polling potential parent for association response
    uint8        security_enb;           ///< Security is enabled
    uint16         short_addr;             ///< Short (network) address of this device
    uint64         ext_addr;               ///< Extended (IEEE) address of this device
        uint8        coord;                  ///< This device is the coordinator
    uint8          curr_channel;           ///< Current active channel


} mac_pib_t;

/*******************************************************************/
/*!
    This struct holds the info for the MAC frame control field.
*/
/*******************************************************************/
typedef struct
{
    uint8      frame_type;     ///< Frame type this frame
    uint8    sec_enb;        ///< Security enabled
    uint8    frame_pending;  ///< Pending frame in the indirect queue
    uint8    ack_req;        ///< ACK requested for this frame
    uint8    pan_id_compr;   ///< Compress PAN ID if PAN ID is same for src and dest
    uint8      frame_ver;      ///< 802.15.4 version this frame supports (2003 or 2006)
} mac_frm_ctrl_t;

/*******************************************************************/
/*!
    This struct contains the fields for the MAC header.
*/
/*******************************************************************/
typedef struct
{
    mac_frm_ctrl_t  mac_frm_ctrl;   ///< Frame control field structure
    uint16             mac_fcf;        ///< Frame control field hex value (value after bit fields are assembled)
    uint8              dsn;            ///< Data sequence number
    uint16             dest_pan_id;    ///< Destination network ID
    address_t       dest_addr;      ///< Destination address
    uint16             src_pan_id;     ///< Source network ID
    address_t       src_addr;       ///< Source address
} mac_hdr_t;

/*******************************************************************/
/*!
    Association request command format
*/
typedef struct
{
    uint8      cap_info;               ///< Capability info for this device
} cmd_assoc_req_t;

/*******************************************************************/
/*!
    Association response command format
*/
typedef struct
{
    uint16     short_addr;             ///< Short address given to the device requesting association
    uint8      assoc_status;           ///< Assocation request status
} cmd_assoc_resp_t;

/*******************************************************************/
/*!
    Coordinator realignment command format - Used for orphan responses
*/
typedef struct
{
    uint16     pan_id;                 ///< Network ID for realignment frame_pending
    uint16     coord_short_addr;       ///< Coordinator or parent's network address
    uint8      channel;                ///< Channel the parent is operating on
    uint16     short_addr;             ///< Network address of the orphan
} cmd_coord_realign_t;

/*******************************************************************/
/*!
    Union of MAC command formats that are used in this stack. Looks like
    only three are used. ha ha ha.
*/
typedef struct
{
    uint8 cmd_id;                                  ///< MAC cmd frame command ID
    union
    {
        cmd_assoc_req_t         assoc_req;      ///< Assocation request cmd frame
        cmd_assoc_resp_t        assoc_resp;     ///< Association response cmd frame
        cmd_coord_realign_t     coord_realign;  ///< Coordinator realign cmd frame
    };
} mac_cmd_t;

/*******************************************************************/
/*!
    This is the PAN descriptor, aka scan descriptor. The PAN descriptors
    are used in the scan list when we do a MAC active scan. When we receive
    the beacon responses, the information in the beacons go into these
    fields.
*/
/*******************************************************************/
typedef struct _pan_descr_t
{
    address_t   coord_addr;     ///< Address of the router sending the beacon
    uint16         coord_pan_id;   ///< Network ID of this network
    uint8          channel;        ///< Channel this network is located on
    uint16         superfrm_spec;  ///< Misc info for this network
    uint8          link_quality;   ///< Quality of link to the sender of this beacon
    uint8          prot_id;        ///< Protocol ID - always 0
    uint8          stack_profile;  ///< Zigbee stack profile
    uint8          prot_ver;       ///< Zigbee Protocol version
    uint8        rtr_cap;        ///< Capacity available for routers to join
    uint8          depth;          ///< Network depth of this router
    uint8        end_dev_cap;    ///< Capacity available for end devices to join
    uint64         ext_pan_id;     ///< Extended PAN ID
    uint8        pot_parent;     ///< Potential Parent for this device
} pan_descr_t;

/*******************************************************************/
/*!
    The MAC data request structure is used to hold the arguments
    for the MAC data request service. The arguments will be used
    to generate the header which will then be stored in the buffer.
*/
/*******************************************************************/
typedef struct
{
  address_t         src_addr;       ///< Src address (usually this device's address)
  address_t         dest_addr;      ///< Dest address
  uint16               src_pan_id;     ///< Src PAN ID
  uint16               dest_pan_id;    ///< Dest PAN ID
  buffer_t          *buf;           ///< Data buffer to be transmitted
  uint8                msdu_handle;    ///< Data handle identifier for this frame
  uint8                tx_options;     ///< Transmission options (ack request, indirect)
} mac_data_req_t;

/*******************************************************************/
/*!
    These are the arguments for the MAC association request function.
*/
/*******************************************************************/
typedef struct
{
  uint8            channel;        ///< Channel of network to join
  address_t     coord_addr;     ///< Address of parent to join
  uint16           coord_pan_id;   ///< PAN ID of network to join
  uint8            capability;     ///< Capability info of this device
} mac_assoc_req_t ;

/*******************************************************************/
/*!
    These are the arguments for the MAC association indication function.
*/
/*******************************************************************/
typedef struct
{
  address_t     dev_addr;       ///< Address of device requesting association
  uint8            capability;     ///< Capability info of device requesting association
} mac_assoc_ind_t;

/*******************************************************************/
/*!
    These are the arguments for the MAC association response function.
*/
/*******************************************************************/
typedef struct
{
  address_t     dev_addr;           ///< IEEE address of child
  uint16           assoc_short_addr;   ///< Network address given to child
  uint8            status;             ///< Status of association request
} mac_assoc_resp_t;

/*******************************************************************/
/*!
    These are the arguments for the MAC association confirm function.
*/
/*******************************************************************/
typedef struct
{
    uint8  status;         ///< Status of scan request
    uint8  scan_type;      ///< Scan type requested
    uint8  *energy_list;   ///< Pointer to the energy scan list
} mac_scan_conf_t;

/*******************************************************************/
/*!
    These are the arguments for the MAC start request function.
*/
/*******************************************************************/
typedef struct
{
    uint16     pan_id;     ///< Start the MAC layer of this device with this PAN ID
    uint8      channel;    ///< Start the MAC layer of this device on this channel
} mac_start_req_t;

/*******************************************************************/
/*!
    This structure is used for the MAC retry queue. When an ACK fails
    to arrive for a transmitted frame, the data from this struct is
    used to re-send the data.
*/
/*******************************************************************/
typedef struct _mac_retry_t
{
    buffer_t            *buf;       ///< Data buffer to be retransmitted
    uint8                  dsn;        ///< Data sequence number of data
    uint8                  retries;    ///< Number of retries remaining
    uint8                  handle;     ///< Data handle identifier for confirmation
    uint8                  expiry;     ///< Time to expiration of this entry
} mac_retry_t;

/*******************************************************************/
/*!
    This struct is used for the MAC indirect queue. Frames are buffered
    in this struct until a sleeping node wakes up and polls this device
    for its data.
*/
/*******************************************************************/
typedef struct _mac_indir_t
{
    buffer_t            *buf;       ///< Data buffer to be transmitted
    uint8                  dsn;        ///< Data sequence number
    uint8                ack_req;    ///< ACK required for this transmission
    address_t           addr;       ///< Destination address of this frame
    uint8                  handle;     ///< Data handle identifier for confirmation
    uint8                  expiry;     ///< Time to expiration of this entry
} mac_indir_t;
/*******************************************************************/
/*!
    This is the MAC protocol control block. It holds variables that
    are used throughout the MAC layer.
*/
/*******************************************************************/
typedef unsigned short clock_time_t;
struct timer {
  clock_time_t start;
  clock_time_t interval;
};
struct etimer {
  struct timer timer;
  struct etimer *next;
  struct process *p;
};
struct ctimer {
  struct ctimer *next;
  struct etimer etimer;
  struct process *p;
  void (*f)(void *);
  void *ptr;
};
typedef struct
{
    uint8              mac_state;          ///< Current state
    struct ctimer   mlme_tmr;           ///< Callback timer shared by MAC managment functions

    // association
    address_t       parent_addr;        ///< Address of parent this device is requesting to join
    uint8              assoc_req_dsn;      ///< DSN of association request

    // channel scanning
    uint16             original_pan_id;    ///< Original PAN ID before channel scan
    uint8              curr_scan_channel;  ///< Current channel being scanned
    uint32             channel_mask;       ///< Channel mask for this scan
    uint8              duration;           ///< Duration of this scan
    uint8              nwk_cnt;            ///< Number of networks found
    uint8              scan_type;          ///< Type of scan to be performed
    uint8            coor_realign_rcvd;  ///< Coordinator alignment received for orphan scan
    uint8              energy_list[MAC_MAX_CHANNELS];  ///< Energy list to store values from energy scan

    // statistics
    uint16             total_xmit;         ///< Total number of transmissions attempted
    uint16             total_fail;         ///< Total number of transmissions failed
} mac_pcb_t;
/**********************************************************/
typedef struct
{


    uint16          Device_PAN_ID;    ///< Destination network ID
    uint16          Device_ShortAddres;      ///< Destination address
    uint16          Device_Logical_Channel;     ///< Source network ID

} Device_Info;

enum TXRX
{
    RX = 0,                     ///< RX enum value
    TX = 1                      ///< TX enum value
};

/*! Main process for the MAC layer */
//PROCESS_NAME(mac_process);

//Customized Functinos
void mac_tx(buffer_t *buf, mac_hdr_t hdr, address_t *addr, uint8 indir, uint8 ack_req, uint8 DSN);
void Transmimit_set(buffer_t *buf, mac_hdr_t hdr);
void Transmmit(buffer_t *buf, uint8 ack_req, uint8 DSN);
//void mac_parse_hdr(buffer_t *buf, mac_hdr_t *hdr);
void mac_parse_cmd(uint8 *rx, mac_cmd_t *cmd);
//void mac_parse_beacon(buffer_t *buf, mac_hdr_t *hdr);
void PAN_SET(mac_hdr_t get_hdr);
void Send_Beacon();
pan_descr_t *mac_desc_get();
mem_ptr_t *mac_scan_alloc();
void mac_association_resp(mac_assoc_resp_t *args);
// mac
void mac_init();
mac_pcb_t *mac_pcb_get();
mac_pib_t *mac_pib_get();
void mac_data_req(mac_data_req_t *req);
void mac_out(buffer_t *buf, uint8 ack_req, uint8 dsn, uint8 handle);
void mac_cmd_handler(mac_cmd_t *cmd, mac_hdr_t *hdr);

// mac reset
void mac_reset_req(uint8 set_default_pib);

void mac_Coordinator_start();
void mac_Device_start();

// mac_queue
void mac_queue_init();
void mac_queue_buf_insert(buffer_t *buf);
buffer_t *mac_queue_buf_pop();
uint8 mac_queue_is_empty();
buffer_t *mac_queue_get_head();
void mac_queue_clear();

//  mac_retry
void mac_retry_init();
void mac_retry_clear();
void mac_retry_add(buffer_t *buf, uint8 dsn, uint8 handle);
void mac_retry_rem(uint8 dsn);
void mac_retry_ack_handler(uint8 dsn);
void mac_retry_periodic(void *ptr);
void mac_retry_stop_tmr();

// mac_gen
uint16 mac_gen_frm_ctrl(const mac_hdr_t *hdr);
void mac_gen_header(buffer_t *buf, mac_hdr_t *hdr);
void mac_gen_cmd_header(buffer_t *buf, mac_hdr_t *hdr, uint8 ack_req, address_t *src_addr, address_t *dest_addr);
void mac_gen_ack(buffer_t *buf, uint8 frm_pend, uint8 dsn);
void mac_gen_cmd(buffer_t *buf, mac_cmd_t *cmd);
uint16 mac_gen_superfrm_spec();
void mac_gen_beacon_frm(buffer_t *buf, mac_hdr_t *hdr);

//mac_parse
void mac_parse_hdr(uint8 *rx, mac_hdr_t *hdr);
//void mac_parse_cmd(buffer_t *buf, mac_cmd_t *cmd);
pan_descr_t  *mac_parse_beacon(uint8 *rx_beacon, mac_hdr_t *hdr);

// mac_indir
void mac_indir_init();
void mac_indir_add(buffer_t *buf, address_t *addr, uint8 ack_req, uint8 dsn, uint8 handle);
void mac_indir_data_req_handler(const address_t *addr);
uint8 mac_indir_frm_pend(const address_t *addr);
void mac_indir_clear();
void mac_indir_periodic();

// mac_scan
void mac_scan_init();
void mac_scan_descr_add(address_t *src_addr, uint16 src_pan_id, uint8 channel, uint16 superframe_spec);
//void mac_scan_descr_free(mem_ptr_t *mem_ptr);
//mem_ptr_t *mac_scan_descr_get_head();
void mac_scan_descr_insert(pan_descr_t *desc);
mem_ptr_t *mac_scan_descr_find_pan(uint16 pan_id);
mem_ptr_t *mac_scan_descr_find_addr(address_t *addr);
void mac_scan_descr_clear();
//mem_ptr_t *mac_scan_descr_find_pan(uint16 pan_id);
//mem_ptr_t *mac_scan_descr_find_addr(address_t *addr);
uint8 *mac_scan_get_energy_list();
void mac_scan_energy();
void mac_scan();
uint8 mac_scan_get_entry_cnt();

// mac_assoc
void mac_assoc_req(mac_assoc_req_t *args);
void mac_assoc_resp(mac_assoc_resp_t *args);
void mac_orphan_resp(uint64 orphan_addr, uint16 short_addr);

// mac_poll
void mac_poll_req();
void mac_poll_timeout();

// mac start
void mac_start_req(const mac_start_req_t *args);
uint16 drvr_get_rand();

void mac_gen_data_frm(buffer_t *buf, mac_hdr_t *hdr);
void mac_gen_data_hdr(buffer_t *buf, mac_hdr_t *hdr, uint8 ack_req,address_t *src_addr, address_t *dest_addr);
void Send_DATA();
//void GET_DATA(uint8 *rx);
#endif /* MAC_LAYER_MAC_H_ */
