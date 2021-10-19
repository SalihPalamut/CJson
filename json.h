#ifndef __JSON_H__
#define __JSON_H__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
typedef struct
{
	char *data;
	unsigned size;

} value_t;
typedef struct json
{
	value_t id;
	value_t val;
	char type;
	struct json * next;
} json_t;



void json_add(json_t ** array, char * key, char * data);
void json_remove(json_t ** array, char * key);
char* json_serialize(const json_t * array);
void json_free(json_t ** array);

#endif

