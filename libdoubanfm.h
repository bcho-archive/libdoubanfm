#ifndef LIBDOUBANFM_H
#define LIBDOUBANFM_H

#include "songs_list.h"

/* user's informations */
typedef struct {
    char *username;
    char *password;
    char *bid;
    char *dbcl2;
    char *uid;
    char *start;
    char *channel_id;
} doubanfm_user_t;

typedef enum {
    LOGIN_FAILED,
    LOGIN_SUCCESS
} login_ret_t;

typedef enum {
    SETUP_FAILED,
    SETUP_SUCCESS
} setup_ret_t;

typedef const char *param_t;

doubanfm_user_t *doubanfm_create_user(char *username, char *password);
void doubanfm_drop_user(doubanfm_user_t *client);
login_ret_t doubanfm_login(doubanfm_user_t *client);
setup_ret_t doubanfm_setup_cookies(doubanfm_user_t *client);
doubanfm_songs_list_t *doubanfm_newlist(doubanfm_user_t *client, \
                                        songs_list_t *history);
/*
void doubanfm_deletesong();
void doubanfm_favsong();
void doubanfm_skipsong();
void doubanfm_playedsong();
void doubanfm_playedlist();
*/
#endif
