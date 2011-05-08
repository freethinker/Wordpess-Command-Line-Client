#include "wppost.h"
#include "debug.h"
#include "utilFuncs.h"

#define NAME "Wordpress Command Line Client"
#define VERSION "0.1"

void
parsePostXml(char *filename, 	wppost_t *post) {
	xmlDocPtr doc = xmlParseFile(filename);
	post->url = getNodeContentByXpath(doc, (xmlChar *) "/post/url");
	post->subject = getNodeContentByXpath(doc, (xmlChar *) "/post/subject");
	post->status = getNodeContentByXpath(doc, (xmlChar *) "/post/status");
	post->categories = getNodeContentByXpath(doc, (xmlChar *) "/post/categories");
	post->tags = getNodeContentByXpath(doc, (xmlChar *) "/post/tags");
	post->body = getNodeContentByXpath(doc, (xmlChar *) "/post/body");
	post->postid = getNodeContentByXpath(doc, (xmlChar *) "/post/postid");
	post->blogid = getNodeContentByXpath(doc, (xmlChar *) "/post/blogid");
	xmlFreeDoc(doc);
}

void
wp_env_init(xmlrpc_env *env, xmlrpc_client **clientP) {
	xmlrpc_env_init(env);
	xmlrpc_client_setup_global_const(env);
	xmlrpc_client_create(env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0,
											 clientP);
	die_if_fault_occurred(env);
}

void
wp_env_clean(xmlrpc_env *env, xmlrpc_client **clientP) {
	xmlrpc_env_clean(env);
	xmlrpc_client_destroy(*clientP);
	xmlrpc_client_teardown_global_const();
}

char *
wpGetBlogId(char *username, char *password, char *url) {
	char *blogid;
	char *methodName = "wp.getUsersBlogs";
	int i;
	int noOfBlogs;
	xmlrpc_env env;
	xmlrpc_client *clientP;
	xmlrpc_value *result, *result2;

	wp_env_init(&env, &clientP);

	/* Make the remote procedure call */
	xmlrpc_client_call2f(&env, clientP, url, methodName, &result,
							"(ss)", username, password);
	die_if_fault_occurred(&env);
	noOfBlogs = xmlrpc_array_size(&env, result);
	if ( noOfBlogs > 1 ) {
		fprintf(stderr, "Currently only single blog wp accounts are supported\n");
		blogid = NULL;
		goto out;
	}
	for(i = 0; i < noOfBlogs; i++) {
		xmlrpc_value *blog_id_xml;
		xmlrpc_array_read_item(&env, result, i, &result2);
		xmlrpc_struct_read_value(&env, result2, "blogid", &blog_id_xml);
		xmlrpc_read_string(&env, blog_id_xml,(const char **) &blogid);
		xmlrpc_DECREF(blog_id_xml);
	}

	/* Dispose of our result value. */
	xmlrpc_DECREF(result2);

out:
	xmlrpc_DECREF(result);

	/* Clean up our error-handling environment. */
	wp_env_clean(&env, &clientP);
	
	return blogid;
}


int main(int argc, const char **argv) {
	
	int retval = 0;
	char *username, *password, *postxml;
	wppost_t post;
	char *blogid = NULL;

	if ( argc != 4) {
		printf("Usage: %s <Path to post xml> <username> <password>\n", argv[0]);
		retval = 1;
		goto out;
	}
	postxml = (char *) argv[1];
	username = (char *) argv[2];
	password = (char *) argv[3];

	parsePostXml(postxml, &post);
	blogid = wpGetBlogId(username, password, post.url);
	if (blogid == NULL) {
		retval = 1;
		goto out;	
	}
	printf("Blog Id - %s\n", blogid); 

out:
	free(blogid);
	return retval;
}
