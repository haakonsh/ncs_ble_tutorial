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

#define BT_UUID_MY_SERIVCE      BT_UUID_DECLARE_128(MY_SERVICE_UUID)
#define BT_UUID_MY_SERIVCE_RX   BT_UUID_DECLARE_128(RX_CHARACTERISTIC_UUID)
#define BT_UUID_MY_SERIVCE_TX   BT_UUID_DECLARE_128(TX_CHARACTERISTIC_UUID)

#define MAX_TRANSMIT_SIZE //TODO figure this out
u8_t data_rx[MAX_TRANSMIT_SIZE];
u8_t data_tx[MAX_TRANSMIT_SIZE];

int my_service_init(void)
{
    int err = 0;

    memset(&data_rx, 0, MAX_TRANSMIT_SIZE);
    memset(&data_tx, 0, MAX_TRANSMIT_SIZE);

    return err;
}

/* This function is called whenever the RX Characteristic has been written to by a Client */
static ssize_t on_receive(struct bt_conn *conn,
			  const struct bt_gatt_attr *attr,
			  const void *buf,
			  u16_t len,
			  u16_t offset,
			  u8_t flags)
{
	printk("Received data, handle %d, conn %p",
		attr->handle, conn);

	return len;
}

/* This function is called whenever a Notification has been sent by the TX Characteristic */
static void on_sent(struct bt_conn *conn, void *user_data)
{
	ARG_UNUSED(user_data);

	printk("Data send, conn %p", conn);
}

/* LED Button Service Declaration and Registration */
BT_GATT_SERVICE_DEFINE(my_service,
BT_GATT_PRIMARY_SERVICE(BT_UUID_MY_SERIVCE),
BT_GATT_CHARACTERISTIC(BT_UUID_MY_SERIVCE_RX,
			       BT_GATT_CHRC_WRITE | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, 
                   NULL, on_receive, NULL),
BT_GATT_CHARACTERISTIC(BT_UUID_MY_SERIVCE_TX,
			       BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ,
                   NULL, NULL, NULL),
BT_GATT_CCC(lbslc_ccc_cfg_changed,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

/* This function sends a notification to a Client with the provided data,
given that the Client Characteristic Control Descripter has been set to Notify (0x1) */
void my_service_send(struct bt_conn *conn, const u8_t *data, uint16_t len)
{
    int err = 0;

    /* 
    The attribute for the TX characteristic is used with bt_gatt_is_subscribed 
    to check whether notification has been enabled by the peer or not.
    Attribute table: 0 = Service, 1 = Primary service, 2 = RX, 3 = TX, 4 = CCC.
    */
    const struct bt_gatt_attr *attr = &my_service.attrs[3]; 

    bt_gatt_notify_params params = {
        .uuid   = BT_UUID_MY_SERIVCE_TX,
        .attr   = attr,
        .data   = data,
        .len    = len,
        .func   = on_sent
    }

    if (bt_gatt_is_subscribed(conn, attr, BT_GATT_CCC_NOTIFY)) {
	    if(bt_gatt_notify_cb(conn, &params)){
            printk("Error, unable to send notification\n");
        }
    }
    else{
        printk("Error, notification not enabled on the selected attribute\n");
    }
}
