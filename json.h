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

typedef union
{
	int* Int;
	char ** Char;
} array_value;

typedef struct
{
	array_value array;
	int size;
	char type;
} array_t;

typedef union
{
	int d;
	double f;
	char *s;
	char c;

	json_t * o;
	array_t *a;
} json_value;

void json_add(json_t ** array, char * key, char * data);
void json_remove(json_t ** array, char * key);
char* json_serialize(const json_t * array);
void json_free(json_t ** array);
json_value json_get_value(const json_t * array, char * key);
int json_deserialized(json_t ** array, char * json_str);
array_t * json_parse_array(char * array);
void json_free_array(array_t ** array);

#endif

