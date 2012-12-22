/* config.h
 * 
 * Holds all configurations.
 *
 */
#ifndef LIBDOUBANFM_CONFIG_H
#define LIBDOUBANFM_CONFIG_H

/* url */
char doubanfm_main_url[] = "http://douban.fm";
char doubanfm_io_url[] = "http://douban.fm/j/mine/playlist?";
char doubanfm_login_url[] = "http://www.douban.com/accounts/login";

/* cookies */
char doubanfm_cookiejar_path[] = "cookies.jar";

/* params */
char *doubanfm_params[] = {
    "aid",
    "sid",
    "channel",
    "du",
    "h",
    "r",
    "rest",
    "uid",
    "type"
};
int doubanfm_params_count = 9;
/* IO type */
char doubanfm_io_type[] = "nbrsep";

/* misc */
char doubanfm_login_form[] = "source=simple&form_email=%s&form_password=%s";

#endif
