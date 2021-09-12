#include "usb.h"
#include "hal_usb.h"
#include "usbd_core.h"
#include "usbd_winusb.h"

#define USBD_VID           0x0D28
#define USBD_PID           0x0204
#define USBD_MAX_POWER     500
#define USBD_LANGID_STRING 1033

#define USBD_WINUSB_VENDOR_CODE 0x20

#define CMSIS_DAP_EP_RECV USB_ENDPOINT_OUT(1)
#define CMSIS_DAP_EP_SEND USB_ENDPOINT_IN(2)

#define CMSIS_DAP_INTERFACE_SIZE (9 + 7 + 7)
#define USB_CONFIG_SIZE          (9 + CMSIS_DAP_INTERFACE_SIZE)

USB_DESC_SECTION const uint8_t rv_dap_plus_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_1, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0100, 0x01),
    /* Configuration 0 */
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x01, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    /* Interface 0 */
    USB_INTERFACE_DESCRIPTOR_INIT(0x00, 0x00, 0x02, 0xFF, 0x00, 0x00, 0x04),
    /* Endpoint OUT 1 */
    USB_ENDPOINT_DESCRIPTOR_INIT(CMSIS_DAP_EP_RECV, USB_ENDPOINT_TYPE_BULK, 0x40, 0x00),
    /* Endpoint IN 2 */
    USB_ENDPOINT_DESCRIPTOR_INIT(CMSIS_DAP_EP_SEND, USB_ENDPOINT_TYPE_BULK, 0x40, 0x00),
    /* String 0 (LANGID) */
    USB_LANGID_INIT(USBD_LANGID_STRING),
    /* String 1 (Manufacturer) */
    0x0E,                       // bLength
    USB_DESCRIPTOR_TYPE_STRING, // bDescriptorType
    'S', 0,                     // wcChar0
    'i', 0,                     // wcChar1
    'p', 0,                     // wcChar2
    'e', 0,                     // wcChar3
    'e', 0,                     // wcChar4
    'd', 0,                     // wcChar5
    /* String 2 (Product) */
    0x18,                       // bLength
    USB_DESCRIPTOR_TYPE_STRING, // bDescriptorType
    'R', 0,                     // wcChar0
    'V', 0,                     // wcChar1
    '-', 0,                     // wcChar2
    'D', 0,                     // wcChar3
    'A', 0,                     // wcChar4
    'P', 0,                     // wcChar5
    ' ', 0,                     // wcChar6
    'P', 0,                     // wcChar7
    'l', 0,                     // wcChar8
    'u', 0,                     // wcChar9
    's', 0,                     // wcChar10
    /* String 3 (Serial Number) */
    0x1A,                       // bLength
    USB_DESCRIPTOR_TYPE_STRING, // bDescriptorType
    '0', 0,                     // wcChar0
    '1', 0,                     // wcChar1
    '2', 0,                     // wcChar2
    '3', 0,                     // wcChar3
    '4', 0,                     // wcChar4
    '5', 0,                     // wcChar5
    'A', 0,                     // wcChar6
    'B', 0,                     // wcChar7
    'C', 0,                     // wcChar8
    'D', 0,                     // wcChar9
    'E', 0,                     // wcChar10
    'F', 0,                     // wcChar11
    /* String 4 (Interface) */
    0x36,                       // bLength
    USB_DESCRIPTOR_TYPE_STRING, // bDescriptorType
    'R', 0,                     // wcChar0
    'V', 0,                     // wcChar1
    '-', 0,                     // wcChar2
    'D', 0,                     // wcChar3
    'A', 0,                     // wcChar4
    'P', 0,                     // wcChar5
    ' ', 0,                     // wcChar6
    'P', 0,                     // wcChar7
    'l', 0,                     // wcChar8
    'u', 0,                     // wcChar9
    's', 0,                     // wcChar10
    ' ', 0,                     // wcChar11
    'C', 0,                     // wcChar12
    'M', 0,                     // wcChar13
    'S', 0,                     // wcChar14
    'I', 0,                     // wcChar15
    'S', 0,                     // wcChar16
    '-', 0,                     // wcChar17
    'D', 0,                     // wcChar18
    'A', 0,                     // wcChar19
    'P', 0,                     // wcChar20
    ' ', 0,                     // wcChar21
    'v', 0,                     // wcChar22
    '2', 0,                     // wcChar23
    '.', 0,                     // wcChar24
    '1', 0,                     // wcChar25
    /* Device Qualifier */
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x10,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
    /* End */
    0x00
};

uint8_t rv_dap_plus_bos_descriptor[] = {
    5, // bLength
    USB_DESCRIPTOR_TYPE_BINARY_OBJECT_STORE,
    WBVAL(40), // wTotalLength
    2, // bNumDeviceCaps
    /* USB 2.0 Extension */
    7, // bLength
    USB_DESCRIPTOR_TYPE_DEVICE_CAPABILITY,
    USB_BOS_CAPABILITY_EXTENSION,
    0x00, 0x00, 0x00, 0x00, // bmAttributes
    /* WinUSB */
    28, // bLength
    USB_DESCRIPTOR_TYPE_DEVICE_CAPABILITY, // bDescriptorType
    USB_BOS_CAPABILITY_PLATFORM, // bDevCapabilityType
    0x00, // bReserved
    0xDF, 0x60, 0xDD, 0xD8,
    0x89, 0x45, 0xC7, 0x4C,
    0x9C, 0xD2, 0x65, 0x9D,
    0x9E, 0x64, 0x8A, 0x9F, // PlatformCapabilityUUID
    /* CapabilityData (Descriptor Set Information) */
    0x00, 0x00, 0x03, 0x06, // dwWindowsVersion
    WBVAL(162), // wDescriptorSetTotalLength
    USBD_WINUSB_VENDOR_CODE, // bVendorCode
    0 // bAltEnumCode
};

uint8_t rv_dap_plus_msosv2_descriptor[] = {
    /* set header */
    WBVAL(10), // wLength
    WBVAL(WINUSB_SET_HEADER_DESCRIPTOR_TYPE),
    0x00, 0x00, 0x03, 0x06, // dwWindowsVersion
    WBVAL(162), // wTotalLength
    /* compatible ID descriptor */
    WBVAL(20), // wLength
    WBVAL(WINUSB_FEATURE_COMPATIBLE_ID_TYPE),
    'W', 'I', 'N', 'U', 'S', 'B', 0, 0, // CompatibleID
    0, 0, 0, 0, 0, 0, 0, 0, // SubCompatibleID
    /* registry property */
    WBVAL(132), // wLength
    WBVAL(WINUSB_FEATURE_REG_PROPERTY_TYPE),
    WBVAL(WINUSB_PROP_DATA_TYPE_REG_MULTI_SZ), // wPropertyDataType
    WBVAL(42), // wPropertyNameLength
    'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0,
    'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0,
    'G', 0, 'U', 0, 'I', 0, 'D', 0, 's', 0, 0, 0, // PropertyName
    WBVAL(80), // wPropertyDataLength
    '{', 0,
    'C', 0, 'D', 0, 'B', 0, '3', 0, 'B', 0, '5', 0, 'A', 0, 'D', 0, '-', 0,
    '2', 0, '9', 0, '3', 0, 'B', 0, '-', 0,
    '4', 0, '6', 0, '6', 0, '3', 0, '-', 0,
    'A', 0, 'A', 0, '3', 0, '6', 0, '-', 0,
    '1', 0, 'A', 0, 'A', 0, 'E', 0, '4', 0, '6', 0, '4', 0, '6', 0, '3', 0, '7', 0, '7', 0, '6', 0,
    '}', 0, 0, 0, 0, 0
};

#include "DAP_config.h"
#include "DAP.h"

static volatile uint16_t USB_RequestIndexI;     // Request  Index In
static volatile uint16_t USB_RequestIndexO;     // Request  Index Out
static volatile uint16_t USB_RequestCountI;     // Request  Count In
static volatile uint16_t USB_RequestCountO;     // Request  Count Out
static volatile uint8_t  USB_RequestIdle;       // Request  Idle  Flag

static volatile uint16_t USB_ResponseIndexI;    // Response Index In
static volatile uint16_t USB_ResponseIndexO;    // Response Index Out
static volatile uint16_t USB_ResponseCountI;    // Response Count In
static volatile uint16_t USB_ResponseCountO;    // Response Count Out
static volatile uint8_t  USB_ResponseIdle;      // Response Idle  Flag

static uint8_t  USB_Request [DAP_PACKET_COUNT][DAP_PACKET_SIZE] __attribute__((section(".bss.USB_IO")));  // Request  Buffer
static uint8_t  USB_Response[DAP_PACKET_COUNT][DAP_PACKET_SIZE] __attribute__((section(".bss.USB_IO")));  // Response Buffer
static uint16_t USB_RespSize[DAP_PACKET_COUNT];                                                           // Response Size

static int usb_dap_msosv2_desc_handler(struct usb_setup_packet *setup, uint8_t **data, uint32_t *transfer_len)
{
    if (setup->bmRequestType_b.Recipient == USB_REQUEST_TO_DEVICE &&
        setup->bRequest == USBD_WINUSB_VENDOR_CODE &&
        setup->wIndex == WINUSB_REQUEST_GET_DESCRIPTOR_SET) {
        // MS OS 2.0 Descriptor
        *data = rv_dap_plus_msosv2_descriptor;
        *transfer_len = sizeof(rv_dap_plus_msosv2_descriptor);
        return 0;
    }
    return -1;
}

static int usb_dap_bos_desc_handler(struct usb_setup_packet *setup, uint8_t **data, uint32_t *transfer_len)
{
    if (setup->bmRequestType_b.Recipient == USB_REQUEST_TO_DEVICE &&
        setup->bRequest == USB_REQUEST_GET_DESCRIPTOR &&
        setup->wValueH == USB_DESCRIPTOR_TYPE_BINARY_OBJECT_STORE) {
        // BOS Descriptor
        *data = rv_dap_plus_bos_descriptor;
        *transfer_len = sizeof(rv_dap_plus_bos_descriptor);
        return 0;
    }
    return -1;
}

static void usb_dap_recv_callback(uint8_t ep)
{
    if (USB_RequestIdle) {
        return;
    }

    uint32_t n = 0;

    usbd_ep_read(ep, USB_Request[USB_RequestIndexI], DAP_PACKET_SIZE, &n);
    if (n != 0U) {
        if (USB_Request[USB_RequestIndexI][0] == ID_DAP_TransferAbort) {
            DAP_TransferAbort = 1U;
        } else {
#if 0
            MSG("Packet Recv, Idx = %d, Type = 0x%02x\r\n", USB_RequestCountI, USB_Request[USB_RequestIndexI][0]);
#endif
            USB_RequestIndexI++;
            if (USB_RequestIndexI == DAP_PACKET_COUNT) {
                USB_RequestIndexI = 0U;
            }
            USB_RequestCountI++;
        }
        usbd_ep_read(ep, NULL, 0, NULL);
    }
    // Start reception of next request packet
    if ((uint16_t)(USB_RequestCountI - USB_RequestCountO) == DAP_PACKET_COUNT) {
        USB_RequestIdle = 1U;
    }
}

static void usb_dap_send_callback(uint8_t ep)
{
    if (USB_ResponseCountI != USB_ResponseCountO) {
        // Load data from response buffer to be sent back
#if 0
        MSG("Packet Send, Idx = %d, Type = 0x%02x\r\n", USB_ResponseCountO, USB_Request[USB_ResponseIndexO][0]);
#endif
        usbd_ep_write(ep, USB_Response[USB_ResponseIndexO], USB_RespSize[USB_ResponseIndexO], NULL);
        USB_ResponseIndexO++;
        if (USB_ResponseIndexO == DAP_PACKET_COUNT) {
            USB_ResponseIndexO = 0U;
        }
        USB_ResponseCountO++;
    } else {
        USB_ResponseIdle = 1U;
    }
}

static void usb_dap_init(uint8_t intf, uint8_t ep_from_host, uint8_t ep_to_host)
{
    static usbd_class_t dap_class = { 0 };
    static usbd_interface_t dap_interface = {
        .class_handler  = NULL,
        .vendor_handler = usb_dap_msosv2_desc_handler,
        .custom_handler = usb_dap_bos_desc_handler,
        .notify_handler = NULL
    };
    static usbd_endpoint_t dap_endpoint_recv = {
        .ep_addr = CMSIS_DAP_EP_RECV,
        .ep_cb   = usb_dap_recv_callback
    };
    static usbd_endpoint_t dap_endpoint_send = {
        .ep_addr = CMSIS_DAP_EP_SEND,
        .ep_cb   = usb_dap_send_callback
    };

    usbd_class_register(&dap_class);
    usbd_class_add_interface(&dap_class, &dap_interface);
    usbd_interface_add_endpoint(&dap_interface, &dap_endpoint_recv);
    usbd_interface_add_endpoint(&dap_interface, &dap_endpoint_send);
    
    USB_RequestIndexI  = 0U;
    USB_RequestIndexO  = 0U;
    USB_RequestCountI  = 0U;
    USB_RequestCountO  = 0U;
    USB_RequestIdle    = 0U;
    USB_ResponseIndexI = 0U;
    USB_ResponseIndexO = 0U;
    USB_ResponseCountI = 0U;
    USB_ResponseCountO = 0U;
    USB_ResponseIdle   = 1U;
}

static void gpio_init(void)
{
    gpio_write(LED_CONNECTED, 1U);
    gpio_write(LED_RUNNING, 1U);
    gpio_set_mode(LED_CONNECTED, GPIO_OUTPUT_MODE);
    gpio_set_mode(LED_RUNNING, GPIO_OUTPUT_MODE);
}

void usb_init(void)
{
    struct device *usb_fs;
    extern struct device *usb_dc_init(void);

    gpio_init();

    usbd_desc_register(rv_dap_plus_descriptor);

    usb_dap_init(0, CMSIS_DAP_EP_RECV & 0x7F, CMSIS_DAP_EP_SEND & 0x7F);

    usb_fs = usb_dc_init();

    if (usb_fs) {
        device_control(usb_fs, DEVICE_CTRL_SET_INT, (void *)(USB_EP1_DATA_OUT_IT | USB_EP2_DATA_IN_IT));
    }

    while (!usb_device_is_configured()) {
        // Simply do nothing
    }
}

void usb_handle(void)
{
    uint32_t n;

    while (USB_RequestCountI != USB_RequestCountO) {

        // Handle Queue Commands
        // n = USB_RequestIndexO;
        // while (USB_Request[n][0] == ID_DAP_QueueCommands) {
        //     USB_Request[n][0] = ID_DAP_ExecuteCommands;
        //     n++;
        //     if (n == DAP_PACKET_COUNT) {
        //         n = 0U;
        //     }
        //     if (n == USB_RequestIndexI) {
        //         flags = osThreadFlagsWait(0x81U, osFlagsWaitAny, osWaitForever);
        //         if (flags & 0x80U) {
        //             break;
        //         }
        //     }
        // }
        if (USB_Request[USB_RequestIndexO][0] == ID_DAP_QueueCommands) {
            // Try to find next DAP_ExecuteCommands
            for (n = (USB_RequestIndexO + 1) % DAP_PACKET_COUNT; n != USB_RequestIndexI; n = (n + 1) % DAP_PACKET_COUNT) {
                if (USB_Request[n][0] != ID_DAP_QueueCommands) {
                    break;
                }
            }
            if (n == USB_RequestIndexI) {
                continue;
            }
            for (uint32_t t = USB_RequestIndexO; t != n; t = (t + 1) % DAP_PACKET_COUNT) {
                USB_Request[t][0] = ID_DAP_ExecuteCommands;
            }
        }

        // Execute DAP Command (process request and prepare response)
        USB_RespSize[USB_ResponseIndexI] =
            (uint16_t)DAP_ExecuteCommand(USB_Request[USB_RequestIndexO], USB_Response[USB_ResponseIndexI]);

        // Update Request Index and Count
        USB_RequestIndexO++;
        if (USB_RequestIndexO == DAP_PACKET_COUNT) {
            USB_RequestIndexO = 0U;
        }
        USB_RequestCountO++;

        if (USB_RequestIdle) {
            if ((uint16_t)(USB_RequestCountI - USB_RequestCountO) != DAP_PACKET_COUNT) {
                USB_RequestIdle = 0U;
                // USBD_EndpointRead(0U, USB_ENDPOINT_OUT(1U), USB_Request[USB_RequestIndexI], DAP_PACKET_SIZE);
            }
        }

        // Update Response Index and Count
        USB_ResponseIndexI++;
        if (USB_ResponseIndexI == DAP_PACKET_COUNT) {
            USB_ResponseIndexI = 0U;
        }
        USB_ResponseCountI++;

        // if (USB_ResponseIdle) {
        //     if (USB_ResponseCountI != USB_ResponseCountO) {
        //     // Load data from response buffer to be sent back
        //         n = USB_ResponseIndexO++;
        //         if (USB_ResponseIndexO == DAP_PACKET_COUNT) {
        //             USB_ResponseIndexO = 0U;
        //         }
        //         USB_ResponseCountO++;
        //         USB_ResponseIdle = 0U;
        //         USBD_EndpointWrite(0U, USB_ENDPOINT_IN(1U), USB_Response[n], USB_RespSize[n]);
        //     }
        // }
    }
}
