#include <stdio.h>
#include <assert.h>

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "config.h"
#include "libdoubanfm.h"
#include "songs_list.h"

void string_cp(char **dest, const char *src)
{
    *dest = malloc(sizeof(char) * (strlen(src) + 1));
    assert(*dest != NULL);
    strcpy(*dest, src);
}

doubanfm_user_t *doubanfm_create_user(char *username, char *password)
{
    doubanfm_user_t *client;

    client = malloc(sizeof(doubanfm_user_t));
    assert(client != NULL);

    string_cp(&client->username, username);
    string_cp(&client->password, password);

    return client;
}

void doubanfm_drop_user(doubanfm_user_t *client)
{
    free(client->username);
    free(client->password);
    free(client->bid);
    free(client->dbcl2);
    free(client->uid);
    free(client->start);
    free(client);
}

static int setup_cookies(doubanfm_user_t *client, CURL *handler)
{
    struct curl_slist *cookies, *i;
    char *value, *name, *tmp;

    curl_easy_getinfo(handler, CURLINFO_COOKIELIST, &cookies);
    client->bid = client->dbcl2 = client->uid = NULL;
    for (i = cookies;i;i = i->next) {
        for (value = strtok(i->data, "\t");(tmp = strtok(NULL, "\t"));) {
            name = value;
            value = tmp;
        }
        value = strtok(strstr(value, "\""), "\"");
        if (strcmp(name, "bid") == 0) {
            string_cp(&client->bid, value);
        } else
        if (strcmp(name, "dbcl2") == 0) {
            string_cp(&client->dbcl2, value);
            string_cp(&client->uid, strtok(value, ":"));
        }
    }

    curl_slist_free_all(cookies);

    if (client->bid && client->dbcl2 && client->uid)
        return 1;
    return 0;
}

login_ret_t doubanfm_login(doubanfm_user_t *client)
{
    CURL *handler;
    char *form;
    int setup_ret;

    /* build post form */
    form = malloc(sizeof(char) * (strlen(doubanfm_login_form) + \
                                  strlen(client->username) + \
                                  strlen(client->password)));
    sprintf(form, doubanfm_login_form, client->username, client->password);

    /* setup curl handler */
    handler = curl_easy_init();
    curl_easy_setopt(handler, CURLOPT_URL, doubanfm_login_url);
    curl_easy_setopt(handler, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(handler, CURLOPT_POST, 1);
    curl_easy_setopt(handler, CURLOPT_POSTFIELDS, form);
    /* provide read & write access */
    curl_easy_setopt(handler, CURLOPT_COOKIEFILE, doubanfm_cookiejar_path);
    curl_easy_setopt(handler, CURLOPT_COOKIEJAR, doubanfm_cookiejar_path);

    curl_easy_perform(handler);
    setup_ret = setup_cookies(client, handler);
    curl_easy_cleanup(handler);

    if (setup_ret)
        return LOGIN_SUCCESS;
    return LOGIN_FAILED;
}

setup_ret_t doubanfm_setup_cookies(doubanfm_user_t *client)
{
    CURL *handler;
    int setup_ret;

    handler = curl_easy_init();
    curl_easy_setopt(handler, CURLOPT_URL, doubanfm_main_url);
    curl_easy_setopt(handler, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(handler, CURLOPT_COOKIEFILE, doubanfm_cookiejar_path);
    curl_easy_perform(handler);
    setup_ret = setup_cookies(client, handler);
    curl_easy_cleanup(handler);

    if (setup_ret)
        return SETUP_SUCCESS;
    return SETUP_FAILED;
}

static size_t response(char *ptr, size_t size, size_t nmemb, char **result)
{
    if (result == NULL)
        *result = malloc(size * nmemb);
    else
        *result = realloc(*result, size * nmemb);
    assert(*result != NULL);
    memcpy(*result, ptr, size * nmemb);

    return size * nmemb;
}

static void doubanfm_io(doubanfm_user_t *client, char *params, char **result)
{
    CURL *handler;
    char *url;

    url = malloc(sizeof(char) * (strlen(doubanfm_io_url) + strlen(params)));
    sprintf(url, "%s", doubanfm_io_url);
    strcat(url, params);

    handler = curl_easy_init();
    curl_easy_setopt(handler, CURLOPT_URL, url);
    curl_easy_setopt(handler, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION, response);
    curl_easy_setopt(handler, CURLOPT_WRITEDATA, result);
    curl_easy_perform(handler);
    curl_easy_cleanup(handler);
    free(url);
}

doubanfm_songs_list_t *doubanfm_newlist(doubanfm_user_t *client)
{
    doubanfm_songs_list_t *list;
    char *raw_json, *params;

    doubanfm_io(client, params, &raw_json);
    list = songs_list_create(raw_json);
}
