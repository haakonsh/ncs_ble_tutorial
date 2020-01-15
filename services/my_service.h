#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr.h>
#include <soc.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#define MY_SERVICE_UUID 0xd4, 0x86, 0x48, 0x24, 0x54, 0xB3, 0x43, 0xA1, \
			            0xBC, 0x20, 0x97, 0x8F, 0xC3, 0x76, 0xC2, 0x75


/** @brief Callback type for when new data is received. */
typedef void (*data_rx_cb_t)(u8_t *data, u8_t length);

/** @brief Callback struct used by the my_service Service. */
struct my_service_cb {
	/** Data received callback. */
	data_rx_cb_t    data_rx_cb;
};

int my_service_init(void);