
#include "gopencl.h"

gint
main (gint argc, gchar **argv)
{
    GopenclPlatform *plat = NULL;
    gchar *str_buf;
    GList *platforms = NULL;
    GError *err = NULL;
    gboolean ret;

    g_type_init();

    ret = gopencl_platform_get_platform_ids (1, &platforms, NULL);
    g_print("ret %d\n", ret);

    g_list_free_full (platforms, g_object_unref);
    platforms = NULL;

    ret = gopencl_platform_get_platform_ids (1, NULL, NULL);
    g_print("ret %d\n", ret);

    ret = gopencl_platform_get_platform_ids (0, &platforms, &err);
    g_print("ret %d: %s\n", ret, err->message);
    g_error_free (err);
    err = NULL;

    ret = gopencl_platform_get_platform_ids (0, NULL, &err);
    g_print("ret %d: %s\n", ret, err->message);
    g_error_free (err);
    err = NULL;

    g_print("--------------\n");

    ret = gopencl_platform_get_platform_ids (5, &platforms, NULL);
    g_print ("%d platforms found\n", g_list_length (platforms));
    plat = GOPENCL_PLATFORM ((g_list_first (platforms))->data);

    g_object_get (plat, "name", &str_buf, NULL);
    g_print ("platform name %s\n", str_buf);
    g_free (str_buf);
    str_buf = NULL;

//     void *cl_plat;
//     g_object_get (plat, "id", &cl_plat, NULL);

    g_object_get (plat, "profile", &str_buf, NULL);
    g_print ("platform profile %s\n", str_buf);
    g_free (str_buf);
    str_buf = NULL;

    g_list_free_full (platforms, g_object_unref);

    return 0;
}
