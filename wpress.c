#include "wpress.h"
#include "debug.h"
#include "utilFuncs.h"

#define NAME "Command Line Wordpress Client"
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
	int i;
	int noOfBlogs;
	xmlrpc_env env;
	xmlrpc_client *clientP;
	char *methodName = "wp.getUsersBlogs";
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

/**
 * mode = 0 for new, and mode = 1 for edit
**/

int
wpCreatePostStruct(xmlrpc_env *env, char *username, char*password, wppost_t *post, xmlrpc_value **paramArray, int mode) {
	int retval = 0;
	xmlrpc_value *blogidP, *postidP, *usernameP, *passwordP, *titleP, *descriptionP, *dateP, *publishP, *categoryP, *contentP, *tokenP;
	char *token;
	char *categories = NULL, *catPtr;
	if ( mode == 0 ) {
		post->blogid = wpGetBlogId(username, password, post->url);
		if (post->blogid == NULL) {
			fprintf(stderr, "NULL Blog Id\n");
			retval = 1;
			goto out;	
		}
		blogidP = xmlrpc_string_new(env, post->blogid);
		postidP = NULL;
	} else {
		blogidP = NULL;
		postidP = xmlrpc_string_new(env, post->postid);
	}
	usernameP = xmlrpc_string_new(env, username);
	passwordP = xmlrpc_string_new(env, password);
	titleP = xmlrpc_string_new(env, post->subject);
	descriptionP = xmlrpc_string_new(env, post->body);
	if(!strcmp(post->status, "draft")) {
		publishP = xmlrpc_bool_new(env, FALSE);
	} else {
		publishP = xmlrpc_bool_new(env, TRUE);
	}
	dateP = xmlrpc_datetime_new_sec(env, time(NULL));
	asprintf(&categories, "%s", post->categories);
	catPtr = categories;
	categoryP = xmlrpc_array_new(env);
	token = strtok(categories, ",");
	tokenP = xmlrpc_string_new(env, token);
	xmlrpc_array_append_item(env, categoryP, tokenP);
	while( (token = strtok( (char *) NULL, ",") ) != NULL ) {
		tokenP = xmlrpc_string_new(env, token);
		xmlrpc_array_append_item(env, categoryP, tokenP);
	}
	free(catPtr);
	contentP = xmlrpc_struct_new(env);
	xmlrpc_struct_set_value(env, contentP, "title", titleP);
	xmlrpc_struct_set_value(env, contentP, "description", descriptionP);
	xmlrpc_struct_set_value(env, contentP, "dateCreated", dateP);
	xmlrpc_struct_set_value(env, contentP, "categories", categoryP);
	/* Populate the Parameter Array */
	*paramArray = xmlrpc_array_new(env);
	if (mode == 0)
		xmlrpc_array_append_item(env, *paramArray, blogidP);
	else
		xmlrpc_array_append_item(env, *paramArray, postidP);
	xmlrpc_array_append_item(env, *paramArray, usernameP);
	xmlrpc_array_append_item(env, *paramArray, passwordP);
	xmlrpc_array_append_item(env, *paramArray, contentP);
	xmlrpc_array_append_item(env, *paramArray, publishP);
out:
	return retval;
}

int 
wpEditPost(char *username, char *password, wppost_t *post) {
	int retval = 0;
	xmlrpc_env env;
	xmlrpc_client *clientP;
	xmlrpc_server_info * serverInfoP;
	xmlrpc_value *paramArrayP = NULL;
	char *methodName = "metaWeblog.editPost";
	xmlrpc_value *resultP;

	wp_env_init(&env, &clientP);
	retval = wpCreatePostStruct(&env, username, password, post, &paramArrayP, 1);
	if (retval != 0) {
		fprintf(stderr, "Error creating post structure, exiting\n");
		goto out;
	}
	serverInfoP = xmlrpc_server_info_new(&env, post->url);
	print_values(&env, paramArrayP);
	xmlrpc_client_call2(&env, clientP, serverInfoP, methodName, paramArrayP, &resultP);
	print_values(&env, resultP);
	xmlrpc_DECREF(resultP);	
out:
	xmlrpc_DECREF(paramArrayP);
	wp_env_clean(&env, &clientP);
	return retval;
}

int 
wpNewPost(char *username, char *password, wppost_t *post) {
	int retval = 0;
	xmlrpc_env env;
	xmlrpc_client *clientP;
	xmlrpc_server_info * serverInfoP;
	xmlrpc_value *paramArrayP = NULL;
	char *methodName = "metaWeblog.newPost";
	xmlrpc_value *resultP;

	wp_env_init(&env, &clientP);
	retval = wpCreatePostStruct(&env, username, password, post, &paramArrayP, 0);
	if (retval != 0) {
		fprintf(stderr, "Error creating post structure, exiting\n");
		goto out;
	}
	serverInfoP = xmlrpc_server_info_new(&env, post->url);
	print_values(&env, paramArrayP);
	xmlrpc_client_call2(&env, clientP, serverInfoP, methodName, paramArrayP, &resultP);
	print_values(&env, resultP);
	xmlrpc_DECREF(resultP);	
out:
	xmlrpc_DECREF(paramArrayP);
	wp_env_clean(&env, &clientP);
	return retval;
}


int main(int argc, const char **argv) {
	
	int retval = 0;
	char *username, *password, *postxml;
	wppost_t post;

	if ( argc != 4) {
		printf("Usage: %s <Path to post xml> <username> <password>\n", argv[0]);
		retval = 1;
		goto out;
	}
	postxml = (char *) argv[1];
	username = (char *) argv[2];
	password = (char *) argv[3];

	parsePostXml(postxml, &post);
	if (post.postid == NULL) {
	/* New Post */
		retval = wpNewPost(username, password, &post);
	} else {
	/* Edit Post */
		retval = wpEditPost(username, password, &post);
	}

	free_wppost(&post);
out:
	return retval;
}
