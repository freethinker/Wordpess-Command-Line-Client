/*
 * =====================================================================================
 *
 *       Filename:  wpress.h
 *
 *    Description:  Command Line Wordpress Client
 *
 *        Version:  1.0
 *        Created:  06/09/2011 08:18:50 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pratik Sinha (freethinker), pratik@humbug.in
 *        Company:  http://www.humbug.in/
 *
 * =====================================================================================
 */

#ifndef _WPPOST_H
#define _WPPOST_H

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>


#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

#define TRUE 1
#define FALSE 0

typedef struct wppost {
	char *url;
	char *subject;
	char *status;
	char *categories;
	char *tags;
	char *body;
	char *blogid;
	char *postid;
} wppost_t;
/*
typedef struct rpcpost {
	xmlrpc_value *blogid;
	xmlrpc_value *postid;
	xmlrpc_value *username;
	xmlrpc_value *password;
	xmlrpc_value *content;
	xmlrpc_value *publish;
} rpcpost_t;
*/
static inline void free_wppost(wppost_t *post) {
	free(post->url);
	free(post->subject);
	free(post->status);
	free(post->categories);
	free(post->tags);
	free(post->body);
	free(post->blogid);
	free(post->postid);
}
/*
static inline void free_rpcpost(rpcpost_t *post) {
	if(post->blogid != NULL)
		xmlrpc_DECREF(post->blogid);
	if(post->postid != NULL)
		xmlrpc_DECREF(post->postid);
	xmlrpc_DECREF(post->username);
	xmlrpc_DECREF(post->password);
	xmlrpc_DECREF(post->content);
	xmlrpc_DECREF(post->publish);
}
*/

#endif /* _WPPOST_H */

