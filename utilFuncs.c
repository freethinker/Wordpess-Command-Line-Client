#include "utilFuncs.h"

char *
getNodeContentByXpath (xmlDocPtr doc, xmlChar *xpath) {
	xmlNodePtr node;
	node = getNodeByXpath(doc, xpath);
	if (node != NULL)
		return ((char *) xmlNodeGetContent(node));
	else
		return NULL;
}

xmlNodePtr
getNodeByXpath (xmlDocPtr doc, xmlChar *xpath) {
	xmlXPathObjectPtr xpathObjPtr;
	xmlNodeSetPtr xpathSetPtr;
	xmlNodePtr node;
	xpathObjPtr = getnodeset(doc, xpath, (xmlNodePtr) NULL);
	if (xpathObjPtr) {
		xpathSetPtr = xpathObjPtr->nodesetval;
		if (xpathSetPtr->nodeTab[0]) {
			node = xpathSetPtr->nodeTab[0];
		} else {
			node = (xmlNodePtr) NULL;
		}
	}	else {
		node = (xmlNodePtr) NULL;
	}
	xmlXPathFreeObject(xpathObjPtr);
	return node;
}

xmlXPathObjectPtr
getnodeset (xmlDocPtr doc, xmlChar *xpath, xmlNodePtr node){

  xmlXPathContextPtr context;
  xmlXPathObjectPtr result;

  context = xmlXPathNewContext(doc);
  if (context == NULL) {
    printf("Error in xmlXPathNewContext\n");
    return NULL;
  }
  if ( node != NULL) {
    context->node = node;
  }
  result = xmlXPathEvalExpression(xpath, context);
  xmlXPathFreeContext(context);
  if (result == NULL) {
    printf("Error in xmlXPathEvalExpression\n");
    return NULL;
  }
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
    xmlXPathFreeObject(result);
    return NULL;
  }
  return result;
}

void
die_if_fault_occurred (xmlrpc_env * const envP) {
	if (envP->fault_occurred) {
		fprintf(stderr, "Failed.  %s\n", envP->fault_string);
		exit(1);
	}
}

int get_int(xmlrpc_env* env, xmlrpc_value* in){
  int i;
  xmlrpc_parse_value(env, in, "i", &i);
  die_if_fault_occurred(env);
  return(i);
}


int get_boolean(xmlrpc_env* env, xmlrpc_value* in){
  int i;
  xmlrpc_parse_value(env, in, "b", &i);
  die_if_fault_occurred(env);
  return(i);
}

double get_double(xmlrpc_env* env, xmlrpc_value* in){
  double d;
  xmlrpc_parse_value(env, in, "d", &d);
  die_if_fault_occurred(env);
  return(d);
}


char* get_timestamp(xmlrpc_env* env, xmlrpc_value* in){
  char *s;
  
  xmlrpc_parse_value(env, in, "8", &s);
  die_if_fault_occurred(env);
  return(s);
}

char* get_string(xmlrpc_env* env, xmlrpc_value* in){
  char* s;

  xmlrpc_parse_value(env, in, "s", &s);
  die_if_fault_occurred(env);
  return(s);
}

char* get_base64(xmlrpc_env* env, xmlrpc_value* in) {
  char *s;

  xmlrpc_parse_value(env, in, "6", &s);
  die_if_fault_occurred(env);
  return(s);
}

void get_array(xmlrpc_env* env, xmlrpc_value* in){
  int i, size = 0;
  xmlrpc_value *el;

  size = xmlrpc_array_size(env, in);
  die_if_fault_occurred(env);

  for(i=0; i < size; i++){
    el = xmlrpc_array_get_item( env, in, i);
    die_if_fault_occurred(env);
    print_values(env, el);
  }
}

void get_struct(xmlrpc_env* env, xmlrpc_value* in){
  int i, size = 0;
  xmlrpc_value *key, *value;

  size = xmlrpc_struct_size(env, in);
  die_if_fault_occurred(env);

  for(i=0; i < size; i++){
    xmlrpc_struct_get_key_and_value(env,
                                    in,
                                    i,
                                    &key,
                                    &value);
    die_if_fault_occurred(env);

    printf("\tkey: ");
    print_values(env, key);
    printf("\tvalue: ");
    print_values(env, value);
  }
}


void print_values( xmlrpc_env* env, xmlrpc_value* in ){

    /* What did we get back? */
    switch (xmlrpc_value_type(in)) {
      case (XMLRPC_TYPE_INT):
        printf("Got an integer: %d\n", get_int(env, in));
        break;
      case (XMLRPC_TYPE_BOOL):
        printf("Got a boolean: %d\n", get_boolean(env, in));
        break;
      case (XMLRPC_TYPE_DOUBLE):
        printf("Got a double: %g\n", get_double(env, in));
        break;
      case (XMLRPC_TYPE_DATETIME):
        printf("Got an ISO8601 timestamp: %s\n", 
              get_timestamp(env, in));
        break;
      case (XMLRPC_TYPE_STRING):
        printf("Got a string: %s\n", get_string(env, in));
        break;
      case (XMLRPC_TYPE_BASE64):
        printf("Got a base64 string: %s\n", 
              get_base64(env, in));
        break;
      case (XMLRPC_TYPE_ARRAY):
        printf("Got an array:\n"); 
        get_array(env, in);
        break;
      case (XMLRPC_TYPE_STRUCT):
        printf("Got a struct\n");
        get_struct(env, in);
        break;
      case (XMLRPC_TYPE_C_PTR):
        printf("Got a C pointer?!\n");
        break;
      case (XMLRPC_TYPE_DEAD):
        printf("Got a 0xDEADr?!\n");
        break;
      default:
        printf("UNKNOWN XML-RPC DATATYPE\n");
		}
}
