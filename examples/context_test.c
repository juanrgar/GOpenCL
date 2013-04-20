// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "gopencl.h"

void
cb_func (GopenclContext *obj,
         const gchararray errinfo,
         gconstpointer private_info,
         gsize cb,
         gpointer user_data)
{
    gchar *user_data_str = (gchar *) user_data;

    g_print("errinfo %s\n", errinfo);
    g_print("private_info %p\n", private_info);
    g_print("cb %ld\n", cb);
    g_print("user_data %p\n", user_data);
    g_print("user_data (string) %s\n", user_data_str);

    g_print("error callback function called\n");
}

gint
main (gint argc, gchar **argv)
{
    GopenclContext *context = NULL;
    GList *devices = NULL;
    GList *devices_get = NULL;
    GError *err = NULL;
    gchar *str_buf = NULL;
    gboolean ret = FALSE;
    char *user_data = g_strdup("this is user data");

    g_type_init();

    ret = gopencl_device_get_all_devices(GOPENCL_DEVICE_TYPE_CPU, &devices, &err);
    g_print("1. ret %d\n", ret);

    context = gopencl_context_new(devices, &err);
    if (context == NULL) {
        g_print("context null\n");
    }
    g_signal_connect(context, "error-ocurred", G_CALLBACK(cb_func), user_data);

    g_object_get(context, "devices", &devices_get, NULL);
    g_print("devices_get %p\n", devices_get);
    g_print("devices %p\n", devices);

    return 0;
}

