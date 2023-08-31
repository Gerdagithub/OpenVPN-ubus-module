#ifndef UBUS_OPENVPN_H
#define UBUS_OPENVPN_H

#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include <syslog.h>

#define MAX_AMOUNT_OF_CLIENTS 100
#define MAX_LENGTH_OF_CLIENT_NAME 20

enum {
    CLIENTS,
    __SERV_MANAGEMENT_MAX,
};

int ubus_loop();
int get_clients(struct ubus_context *ctx, struct ubus_object *obj,
		        struct ubus_request_data *req, const char *method,
		        struct blob_attr *msg);


static const struct ubus_method device_methods[] = {
    UBUS_METHOD_NOARG("clients", get_clients),
};

static struct ubus_object_type device_object_type =
	UBUS_OBJECT_TYPE("openvpn-server", device_methods);


static struct ubus_object devices_object = {
	.name = "openvpn_server",
	.type = &device_object_type,
	.methods = device_methods,
	.n_methods = ARRAY_SIZE(device_methods),
};

#endif