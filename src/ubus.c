#include "ubus.h"

extern volatile bool run;

char clients[MAX_LENGTH_OF_CLIENT_NAME][MAX_AMOUNT_OF_CLIENTS];
int amountOfClients = 0;

int ubus_loop()
{
    struct ubus_context *ctx;
	int ret = 0;

	ret = uloop_init();
	if (ret){
		syslog(LOG_USER | LOG_ERR, "uloop_init failed");
		return ret;
	}

	ctx = ubus_connect(NULL);
	if (!ctx) {
		syslog(LOG_USER | LOG_ERR, "Failed to connect to ubus");
		ret = -1;
		goto cleanUp;
	}

	ubus_add_uloop(ctx);

	ret = ubus_add_object(ctx, &devices_object);
	if (ret){
		syslog(LOG_USER | LOG_ERR, "ubus_add_object failed");
		goto cleanUp;
	}

	ret = uloop_run();
	if (ret){
		syslog(LOG_USER | LOG_ERR, "uloop_run failed");
		goto cleanUp;
	}
 
cleanUp:
	uloop_done();
	if (ctx)
		ubus_free(ctx);

    return ret;
}

int get_clients(struct ubus_context *ctx, struct ubus_object *obj,
		        struct ubus_request_data *req, const char *method,
		        struct blob_attr *msg)
{
	struct blob_buf buf = {};
    blob_buf_init(&buf, 0);

	void *clientsArray;
    clientsArray = blobmsg_open_array(&buf, "clients");

	for (int i = 0; i < amountOfClients; i++){
        blobmsg_add_string(&buf, NULL, clients[i]);
	}

	blobmsg_close_array(&buf, clientsArray);

	ubus_send_reply(ctx, req, buf.head);
    blob_buf_free(&buf);
}