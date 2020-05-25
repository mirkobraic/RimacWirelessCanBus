#ifndef KVASERCONSTANTS_H
#define KVASERCONSTANTS_H

namespace KvaserConstants {
enum ResponseStatus {
    canOK                       = 0,        ///< Normal successful completion
    canERR_PARAM                = -1,       ///< Error in one or more parameter
    canERR_NOMSG                = -2,       ///< There were no messages to read
    canERR_NOMEM                = -3,       ///< Out of memory
    canERR_NOCHANNELS           = -5,       ///< No channels available
    canERR_TIMEOUT              = -7,       ///< Timeout occurred
    canERR_INVHANDLE            = -10,      ///< Handle is invalid
    canERR_HARDWARE             = -15,      ///< A hardware error has occurred
    canERR_NOT_IMPLEMENTED      = -32,      ///< The requested feature or function is not implemented
    canERR_DEVICE_FILE          = -33,      ///< An error has occured when trying to access a file on the device
    canERR_SCRIPT_FAIL          = -39,      ///< A script has failed
    canERR_SCRIPT_WRONG_VERSION = -40,      ///< The t script version dosen't match the version that the device firmware supports
    canERR_INVALID_PASSWORD     = -128,
    canERR_NO_SUCH_FUNCTION     = -129,
    canERR_NOT_AUTHORIZED       = -130,
    canERR_INVALID_SESSION      = -131,
    NETWORK_ERR                 = -200
};

enum OpenFlags {
    canOPEN_EXCLUSIVE           = 0x0008,   ///< Don't allow sharing of this channel between applications.
    canOPEN_REQUIRE_EXTENDED    = 0x0010,   ///< The call will fail if the specified circuit doesn't allow extended CAN, if no frame-type flag is specified in a call to canWrite, it is assumed that extended CAN should be used.
    canOPEN_ACCEPT_VIRTUAL      = 0x0020,   ///< Allow opening of virtual channels as well as physical channels.
    canOPEN_OVERRIDE_EXCLUSIVE  = 0x0040,   ///< Open the channel even if it is opened for exclusive access already.
    canOPEN_REQUIRE_INIT_ACCESS = 0x0080,   ///< Fail the call if the channel cannot be opened with init access. Init access means that the CAN handle can set bit rate and CAN driver mode. At most one CAN handle may have init access to any given channel.
    canOPEN_NO_INIT_ACCESS      = 0x0100,   ///< Don't open the handle with init access.
    canOPEN_ACCEPT_LARGE_DLC    = 0x0200,   ///< The channel will accept messages with DLC greater than 8.
    canOPEN_CAN_FD              = 0x0400,   ///< The channel will use the CAN FD protocol, ISO compliant.
    canOPEN_CAN_FD_NONISO       = 0x0800,   ///< The channel will use the CAN FD NON-ISO protocol.
};

enum MsgFlags {
    canMSG_STD          = 0x0002,   ///< Message has a standard (11-bit) identifier
    canMSG_EXT          = 0x0004,   ///< Message has an extended (29-bit) identifier
    canMSG_WAKEUP       = 0x0008,   ///< Message is a WAKEUP message (SWC hardware.)
    canMSG_NERR         = 0x0010,   ///< NERR was active during the message (TJA1054 hardware)
    canMSG_ERROR_FRAME  = 0x0020,   ///< Message represents an error frame.
    canMSG_TXACK        = 0x0040,   ///< Message is a TX ACK (msg has really been sent)
    canMSG_TXRQ         = 0x0080,   ///< Message is a TX REQUEST (msg was transfered to the chip)
    canMSG_DELAY_MSG    = 0x0100,   ///< Message is NOT sent on the bus. The transmission of messages are delayed. The dlc specifies the delay in milliseconds (1..1000).

    // single shot flags:
    canMSG_SINGLE_SHOT  = 0x1000000,    ///< Message is Single Shot, try to send once, no retransmission. This flag can only be used with transmitted messages.
    canMSG_TXNACK       = 0x2000000,    ///< Message is a failed Single Shot, message was not sent. This flag is only used with received messages.
    canMSG_ABL          = 0x4000000     ///< Only together with canMSG_TXNACK, Single shot message was not sent because arbitration was lost. This flag is only used with received messages.
};
}

#endif // KVASERCONSTANTS_H
