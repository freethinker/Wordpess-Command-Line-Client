#ifndef _WPPOST_H
#define _WPPOST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>

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


#endif /* _WPPOST_H */

