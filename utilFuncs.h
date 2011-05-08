#ifndef _UTILFUNCS_H
#define _UTILFUNCS_H

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

xmlXPathObjectPtr getnodeset (xmlDocPtr doc, xmlChar *xpath, xmlNodePtr node);
xmlNodePtr getNodeByXpath (xmlDocPtr doc, xmlChar *xpath);
char *getNodeContentByXpath (xmlDocPtr doc, xmlChar *xpath);


void die_if_fault_occurred (xmlrpc_env * const envP);
int get_int(xmlrpc_env* env, xmlrpc_value* in);
int get_boolean(xmlrpc_env* env, xmlrpc_value* in);
double get_double(xmlrpc_env* env, xmlrpc_value* in);
char* get_timestamp(xmlrpc_env* env, xmlrpc_value* in);
char* get_string(xmlrpc_env* env, xmlrpc_value* in);
char* get_base64(xmlrpc_env* env, xmlrpc_value* in); 
void get_array(xmlrpc_env* env, xmlrpc_value* in);
void get_struct(xmlrpc_env* env, xmlrpc_value* in);
void print_values( xmlrpc_env* env, xmlrpc_value* in );

#endif /* _UTILFUNCS_H */

