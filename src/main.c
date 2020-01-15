/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/printk.h>
#include <misc/byteorder.h>
#include <zephyr.h>
#include <gpio.h>
#include <soc.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include "../services/my_service.h"

#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

static K_SEM_DEFINE(ble_init_ok, 0, 1);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, LBS_UUID_SERVICE),
};

data_rx_cb_t data_rx_cb(u8_t *data, u8_t length)
{
	//Do stuff
}

static struct my_service_cb my_service_callbacs = {
	.data_rx_cb    = data_rx_cb,
};

static void connected(struct bt_conn *conn, u8_t err)
{
	if (err) {
		printk("Connection failed (err %u)\n", err);
		return;
	}

	printk("Connected\n");
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);
}

static bool le_param_req(struct bt_conn *conn, struct bt_le_conn_param *param)
{
	//If acceptable params, return true, otherwise return false.
	return true; 
}

static void le_param_updated(struct bt_conn *conn, u16_t interval, u16_t latency, u16_t timeout)
{
	printk("BLE link parameters updated; Connection handle:%u, interval:%u, latency:%u, timeout:%u \n",
			conn.handle, interval, latency, timeout);
}

static struct bt_conn_cb conn_callbacks = {
	.connected			= connected,
	.disconnected   	= disconnected,
	.le_param_req		= le_param_req,
	.le_param_updated	= le_param_updated
};

static void bt_ready(int err)
{
	if (err) {
		printk("BLE init failed with error code %d\n", err);
		return;
	}

	//Configure connection callbacks
	bt_conn_cb_register(&conn_callbacks);

	//Initalize services
	err = my_service_init(&my_service_callbacs);

	if (err) {
		printk("Failed to init LBS (err:%d)\n", err);
		return;
	}

	//Start advertising
	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");

	k_sem_give(&ble_init_ok);
}


static void error(void)
{
	while (true) {
		printk("Error!\n");
		/* Spin for ever */
		k_sleep(1000);
	}
}

void main(void)
{
	
	int err = 0;

	printk("Starting Nordic BLE peripheral tutorial\n");

	
	err = bt_enable(bt_ready);

	if (err) {
		error(); //Catch error
	}
	
	/* 	Bluetooth stack should be ready in less than 100 msec. 								\
																							\
		We use this semaphore to wait for bt_enable to call bt_ready before we proceed 		\
		to the main loop. By using the semaphore to block execution we allow the RTOS to 	\
		execute other tasks while we wait. */	
	err = k_sem_take(&ble_init_ok, K_MSEC(100));

	if (!err) {
		printk("Bluetooth initialized\n");
	} else {
		printk("BLE initialization did not complete in time\n");
		error(); //Catch error
	}

	for (;;) {
		// Main loop
	}
}

