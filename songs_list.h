#ifndef SONGS_LIST_H
#define SONGS_LIST_H

#include "json.h"

typedef struct _song_t {
    char *picture;
    char *albumtitle;
    char *company;
    double rating_avg;
    char *public_time;
    char *ssid;
    char *album;
    int  like;
    char *artist;
    char *url;
    char *title;
    char *subtype;
    int length;
    char *sid;
    char *aid;
} song_t;

typedef struct _songs_list_t {
    song_t *songs;
    int length;
} songs_list_t;

typedef struct _doubanfm_songs_list_t {
    songs_list_t *r;
    songs_list_t *p;
} doubanfm_songs_list_t;


doubanfm_songs_list_t *songs_list_create(char *raw);
void songs_list_free(doubanfm_songs_list_t *list);
song_t *songs_list_get(doubanfm_songs_list_t *list);
void songs_list_finish(doubanfm_songs_list_t *list);
int songs_list_is_finish(doubanfm_songs_list_t *list);
char *songs_list_format_list(songs_list_t *list, char *verb);
#endif
