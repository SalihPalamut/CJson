#include "json.h"

static char get_type(const char *data)
{
	const char *p = data;
	size_t size = strlen(data);
	char type = 's';

	if(size == 4)
	{
		if(!strcmp("true", data))return 'b';
	}
	if(size == 4)
	{
		if(!strcmp("null", data))return 'b';
	}
	if(size == 5)
	{
		if(!strcmp("false", data))return 'b';
	}

	if(*p == '{' && *(p + size - 1) == '}')
	{
		return 'o';
	}
	if(*p == '[' && *(p + size - 1) == ']')
	{
		return 'a';
	}
	if(*p == '+' || *p == '-')
	{
		p++;
		size--;
	}

	if(isxdigit(*p) || *p == '.' || *p == '0')
	{
		type = 'd';
		if(size > 2)
		{
			if(*(p + 1) == 'x')
			{
				type = 'h';
				p += 2;
				while(isxdigit(*p))
				{
					p++;
				}
				if(*p)type = 's';
				return type;
			}
		}

		if(*p == '.')
			type = 'f';
		p++;
		while(isdigit(*p))
		{
			p++;
		}
		if(*p == '.' && type != 'f')
		{
			p++;
			type = 'f';
			while(isdigit(*p))
			{
				p++;
				if(*p == 'e' || *p == 'E')
				{
					p++;
					if(*p == '+' || *p == '-')
					{
						p++;
						if(!*p)type = 's';
					}
				}
			}
		}
		if(*p)type = 's';
	}

	if(strlen(data) == 1)type = isdigit(data[0]) > 0 ? 'd' : 'c';
	return type;
}

void json_add(json_t ** array, char * key, char * data)
{
	json_t *new_node = 0;
	if(*array == NULL)
	{
		(*array) = (json_t *)calloc(1, sizeof(json_t));
		new_node = (*array);
	}
	else
	{
		new_node = (json_t *)calloc(1, sizeof(json_t));
		new_node->next = *array;
		*array = new_node;
	}

	new_node->id.size = strlen(key);
	new_node->id.data = calloc(1, new_node->id.size + 1);
	memcpy(new_node->id.data, key, new_node->id.size);

	new_node->val.size = strlen(data);
	new_node->val.data = calloc(1, new_node->val.size + 1);
	memcpy(new_node->val.data, data, new_node->val.size);

	new_node->type = get_type(data);

}

void json_remove(json_t ** array, char * key)
{

	if(!key)return;
	json_t * currP, *prevP;

	prevP = NULL;

	for(currP = *array; currP; prevP = currP, currP = currP->next)
	{

		if(!strcmp(currP->id.data, key))
		{
			if(prevP == NULL)
			{
				*array = currP->next;
			}
			else
			{
				prevP->next = currP->next;
			}

			free(currP->id.data);
			free(currP->val.data);
			free(currP);
			return;
		}
	}
}

void json_free(json_t ** array)
{
	json_t * current = *array;
	while(*array)
	{
		free((*array)->id.data);
		free((*array)->val.data);
		*array = (*array)->next;
		free(current);
		current = *array;
	}
}

static size_t get_length(const json_t * array)
{
	const json_t * current = array;
	size_t count = 0;
	while(current)
	{
		count += current->id.size + current->val.size + 7;
#ifdef Debug
		printf("key %s value %s\n", current->id.data, current->val.data);
#endif
		current = current->next;
	}
	return count;
}

char* json_serialize(const json_t * array)
{
	size_t max_length = get_length(array);
	char * serialize = (char*)calloc(1, max_length + 1);
	size_t i = 0;
	serialize[i++] = '{';

	const json_t * current = array;

	while(current)
	{
		serialize[i++] = '"';
		memcpy(&serialize[i], current->id.data, current->id.size);
		i += current->id.size;
		memcpy(&serialize[i], "\":", 2);
		i += 2;

		if(current->type == 's' || current->type == 'c')
			serialize[i++] = '"';
		memcpy(&serialize[i], current->val.data, current->val.size);
		i += current->val.size;
		if(current->type == 's' || current->type == 'c')
			serialize[i++] = '"';

		current = current->next;
		if(current)
			serialize[i++] = ',';
	}
	serialize[i++] = '}';
	serialize[i] = 0;
#ifdef Debug
	printf("Max Length %ld\nSerialize %s", max_length, serialize);
#endif
	return serialize;
}



