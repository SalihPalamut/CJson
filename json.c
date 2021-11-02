#include "json.h"

static int atoi_helper(const char *s)
{
	uint8_t multipler = 10;
	uint8_t negative = 0;
	int number = 0;
	while(isspace(*s))
	{
		s++;
	}


	if(*s == '-')
	{

		negative = 1;
		s++;
	}


	if(strlen(s) >= 2)
	{

		if(!strncmp(s, "0x", 2))
		{
			s++;
			s++;
			multipler = 16;
		}
		else if(*s == '0')
		{
			multipler = 8;
			s++;
		}

	}

	if(multipler == 10 || multipler == 8)
	{
		while(isdigit(*s))
		{
			number = multipler * number - (*(s++) - 0x30);
		}
	}

	if(multipler == 16)
	{
		while(isxdigit(*s))
		{
			if(isdigit(*s))
				number = multipler * number - (*(s++) - 0x30);
			else
				number = multipler * number - (*(s++) - 0x57);
		}
	}
	return negative ? number : -number;
}

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

void json_free_array(array_t ** array)
{

	array_t *P = *array;
	if(P->type == 's')
	{
		for(int i = 0; i < P->size; i++)
			free(P->array.Char[i]);

	}

	free(P->array.Char);
	free(P);

}

array_t * json_parse_array(char * array)
{
	array_t * a = 0;
	char *p = array;
	int n = 0, m = 0;
	char c = 'd', *tmp;
	if(*(p + 1) == '"')c = 's';

	switch(c)
	{
		case 's':
			a = calloc(1, sizeof(array_t));
			p++;
			while(*p)
			{
				m = 0;
				a->array.Char = (char**)realloc(a->array.Char, (n + 1) * sizeof(char*));
				while(*p != ',' && *p != ']')
				{
					m++;
					p++;
				}
				a->array.Char[n] = calloc(1, m + 1);
				memcpy(a->array.Char[n], (p - m + 1), m - 2);
				n++;
				p++;

			}

			break;
		case 'd':
			a = calloc(1, sizeof(array_t));
			p++;
			while(*p)
			{
				m = 0;
				a->array.Int = (int*)realloc(a->array.Int, (n + 1) * sizeof(int*));
				while(*p != ',' && *p != ']')
				{
					m++;
					p++;
				}
				tmp = calloc(1, m + 1);
				memcpy(tmp, (p - m), m);
				a->array.Int[n] = atoi_helper(tmp);
				n++;
				p++;
				free(tmp);
			}
			break;
	}
	a->type = c;
	a->size = n;
	return a;
}

int json_deserialized(json_t ** array, char * json_str)
{
	char * p = json_str;
	int valid = -1, n = 0, i = 0;;
	char *data[2];
	while(*p)
	{

		switch(*p)
		{
			case '{':
				if(valid == -1)
					valid = 0;
				else if(!valid)
				{

					p++;
					n = 0;
					while(*p != '}')
					{
						n++;
						p++;
					}
					n++;

					data[i] = calloc(1, n + 2);
					memcpy(data[i], (p - n), n + 1);
				}
				break;
			case '}':
				valid = 1;
				i++;
				break;
			case '"':
				p++;
				n = 0;
				while(*p != '"')
				{
					n++;
					p++;
				}
				data[i] = calloc(1, n + 1);
				memcpy(data[i], (p - n), n);
				break;

			case '[':
				//p++;
				n = 0;
				while(*p != ']')
				{
					n++;
					p++;
				}
				data[i] = calloc(1, n + 2);
				memcpy(data[i], (p - n), n + 1);
				break;

			case ':':
				i++;
				break;
			case ',':
				i++;
				break;
			default:

				n = 0;
				while(*p != ',' && *p != '}')
				{
					n++;
					p++;
				}
				data[i] = calloc(1, n + 1);
				memcpy(data[i], (p - n), n);
				i++;
				break;

		}

		p++;
		if(i == 2)
		{
#ifdef Debug
			printf("%s %s", data[0], data[1]);
#endif // Debug
			json_add(array, data[0], data[1]);
			free(data[0]);
			free(data[1]);
			i = 0;
		}

	}

	return valid;
}



json_value json_get_value(const json_t * array, char * key)
{
	const json_t * current = array;

	array_t *a = 0;
	json_t * o = 0;
	char *s = 0;

	while(current)
	{

		if(!strcmp(current->id.data, key))
		{

			switch(current->type)
			{
				case 'b':
				case 'c':
					return (json_value)current->val.data[0];
				case 'd':
					return (json_value)atoi_helper(current->val.data);
				case 'f':
					return (json_value)atof(current->val.data);
				case 'h':
					return (json_value)((int)atoi_helper(current->val.data));
				case 'o':
					json_deserialized(&o, current->val.data);
					return (json_value)o;
				case 'a':
					a = json_parse_array(current->val.data);
					return (json_value)a;
				case 's':
					s = calloc(1, current->val.size + 1);
					memcpy(s, current->val.data, current->val.size);
					return (json_value)s;
			}

		}

		current = current->next;
	}
	return (json_value)0;
}


