// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "gopencl.h"

gint
main (gint argc, gchar **argv)
{
    GopenclPlatform *plat = NULL;
    gchar *str_buf;
    GList *platforms = NULL;
    GError *err = NULL;
    gboolean ret = FALSE;

    g_type_init();

    ret = gopencl_platform_get_platform_ids (&platforms, NULL);
    g_print("1. ret %d\n", ret);

    g_list_free_full (platforms, g_object_unref);
    platforms = NULL;

    ret = gopencl_platform_get_platform_ids (NULL, NULL);
    g_print("2. ret %d\n", ret);

    ret = gopencl_platform_get_platform_ids (NULL, &err);
    g_print("4. ret %d: %s\n", ret, err->message);
    g_error_free (err);
    err = NULL;

    g_print("--------------\n\n\n");

    ret = gopencl_platform_get_platform_ids (&platforms, NULL);
    g_print ("%d platforms found\n", g_list_length (platforms));
    plat = GOPENCL_PLATFORM ((g_list_first (platforms))->data);

    g_object_get (plat, "profile", &str_buf, NULL);
    g_print ("\tPlatform profile %s\n", str_buf);
    g_free (str_buf);

    g_object_get (plat, "version", &str_buf, NULL);
    g_print ("\tPlatform version %s\n", str_buf);
    g_free (str_buf);

    g_object_get (plat, "name", &str_buf, NULL);
    g_print ("\tPlatform name %s\n", str_buf);
    g_free (str_buf);

    g_object_get (plat, "vendor", &str_buf, NULL);
    g_print ("\tPlatform vendor %s\n", str_buf);
    g_free (str_buf);

    g_object_get (plat, "extensions", &str_buf, NULL);
    g_print ("\tPlatform extensions %s\n", str_buf);
    g_free (str_buf);

    g_list_free_full (platforms, g_object_unref);

    return 0;
}

