#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <zephyr.h>
#include <soc.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include "my_service.h"

#define BT_UUID_MY_SERIVCE      BT_UUID_DECLARE_128(MY_SERVICE_UUID)
#define BT_UUID_MY_SERIVCE_RX   BT_UUID_DECLARE_128(RX_CHARACTERISTIC_UUID)
#define BT_UUID_MY_SERIVCE_TX   BT_UUID_DECLARE_128(TX_CHARACTERISTIC_UUID)

#define MAX_TRANSMIT_SIZE 240//TODO figure this out

u8_t data_rx[MAX_TRANSMIT_SIZE];
u8_t data_tx[MAX_TRANSMIT_SIZE];

int my_service_init(void)
{
    int err = 0;

    memset(&data_rx, 0, MAX_TRANSMIT_SIZE);
    memset(&data_tx, 0, MAX_TRANSMIT_SIZE);

    return err;
}
