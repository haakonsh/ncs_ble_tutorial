#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/printk.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <soc.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#define MAX_TRANSMIT_SIZE //TODO figure this out
u8_t data_rx[MAX_TRANSMIT_SIZE];
u8_t data_tx[MAX_TRANSMIT_SIZE];


int my_service_init(void)
{
    int err = 0;
   
    return err;
}

/* LED Button Service Declaration */
BT_GATT_SERVICE_DEFINE(lbs_svc,
BT_GATT_PRIMARY_SERVICE(BT_UUID_LBS),
#ifdef CONFIG_BT_GATT_LBS_POLL_BUTTON
	BT_GATT_CHARACTERISTIC(BT_UUID_LBS_BUTTON,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, read_button, NULL,
			       &button_state),
#else
	BT_GATT_CHARACTERISTIC(BT_UUID_LBS_BUTTON,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, NULL, NULL, NULL),
#endif
	BT_GATT_CCC(lbslc_ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_LBS_LED,
			       BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_WRITE,
			       NULL, write_led, NULL),
);