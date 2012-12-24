#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "json.h"
#include "utils.h"
#include "songs_list.h"

void string_attribute_copy(char **dest, json_value *src)
{
    assert(src->type == json_string);
    *dest = malloc(sizeof(char) * src->u.string.length);
    strcpy(*dest, src->u.string.ptr);
}

doubanfm_songs_list_t *songs_list_create(char *raw)
{
    int i, j;
    char *name;
    json_value *song, *value, *songs, *raw_json;
    song_t *cur;
    doubanfm_songs_list_t *list;

    raw_json = json_parse(raw);
    songs = raw_json->u.object.values[1].value;

    list = malloc(sizeof(doubanfm_songs_list_t));
    assert(list != NULL);
    list->r = malloc(sizeof(songs_list_t));
    assert(list->r != NULL);
    list->r->songs = malloc(sizeof(song_t) * songs->u.array.length);
    list->r->length = songs->u.array.length;
    list->p = malloc(sizeof(songs_list_t));
    assert(list->p != NULL);
    list->p->songs = malloc(sizeof(song_t) * songs->u.array.length);
    list->p->length = 0;

    assert(songs->type == json_array);
    for (i = 0;i < songs->u.array.length;i++) {
        song = songs->u.array.values[i];
        assert(song->type == json_object);
        cur = list->r->songs + i;
        for (j = 0;j < song->u.object.length;j++) {
            name = song->u.object.values[j].name;
            value = song->u.object.values[j].value;
            /* string */
            if (strcmp(name, "picture") == 0) {
                string_attribute_copy(&cur->picture, value);
            } else
            if (strcmp(name, "albumtitle") == 0) {
                string_attribute_copy(&cur->albumtitle, value);
            } else
            if (strcmp(name, "company") == 0) {
                string_attribute_copy(&cur->company, value);
            } else
            if (strcmp(name, "public_time") == 0) {
                string_attribute_copy(&cur->public_time, value);
            } else 
            if (strcmp(name, "ssid") == 0) {
                string_attribute_copy(&cur->ssid, value);
            } else
            if (strcmp(name, "album") == 0) {
                string_attribute_copy(&cur->album, value);
            } else
            if (strcmp(name, "artist") == 0) {
                string_attribute_copy(&cur->artist, value);
            } else
            if (strcmp(name, "title") == 0) {
                string_attribute_copy(&cur->title, value);
            } else
            if (strcmp(name, "subtype") == 0) {
                string_attribute_copy(&cur->subtype, value);
            } else
            if (strcmp(name, "sid") == 0) {
                string_attribute_copy(&cur->sid, value);
            } else
            if (strcmp(name, "aid") == 0) {
                string_attribute_copy(&cur->aid, value);
            } else
            if (strcmp(name, "url") == 0) {
                string_attribute_copy(&cur->url, value);
            }
            /* int */
            if (strcmp(name, "like") == 0) {
                cur->like = value->u.integer;
            } else
            if (strcmp(name, "length") == 0) {
                cur->length = value->u.integer;
            } else
            /* double */
            if (strcmp(name, "rating_avg") == 0) {
                cur->rating_avg = value->u.dbl;
            }
        }
    }

    json_value_free(raw_json);
    return list;
}

void song_free(song_t *song)
{
    free(song->picture);
    free(song->albumtitle);
    free(song->company);
    free(song->public_time);
    free(song->ssid);
    free(song->album);
    free(song->artist);
    free(song->url);
    free(song->title);
    free(song->subtype);
    free(song->sid);
    free(song->aid);
    free(song);
}

void songs_list_free(doubanfm_songs_list_t *list)
{
    int i;

    for (i = 0;i < list->r->length;i++)
        song_free(&list->r->songs[i]);
    free(list->r);
    free(list->p);
    free(list);
}

song_t *songs_list_get(doubanfm_songs_list_t *list)
{
    if (!songs_list_is_finish(list))
        return list->r->songs + list->r->length - 1;
    return NULL;
}

void songs_list_finish(doubanfm_songs_list_t *list)
{
    if (songs_list_is_finish(list))
        return;
    list->p->songs[list->p->length++] = list->r->songs[--list->r->length];
}

int songs_list_is_finish(doubanfm_songs_list_t *list)
{
    return (list->r->length <= 0);
}

char *songs_list_format_list(songs_list_t *list, char *verb)
{
    char *ret, *prev, *cur, *id;
    int i;
    song_t *song;

    ret = malloc(sizeof(char) * 0);
    for (i = 0, prev = ret;i < list->length;i++) {
        song = (list->songs + i);
        if (verb != NULL)
            /* FIXME sid or aid or other? */
            id = join(song->sid, verb, ':');
        else
            id = song->sid;
        ret = join(prev, id, '|');
        free(prev);
        prev = ret;
        if (verb != NULL)
            free(id);
    }

    return ret;
}
