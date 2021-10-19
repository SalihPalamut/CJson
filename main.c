#include <stdio.h>
#include <stdlib.h>
#include "json.h"

int main()
{
	char * serialized = 0;
	json_t *t = 0;
	json_add(&t, "a", "key");
	json_add(&t, "b", "55");
	serialized = json_serialize(t);
	json_free(&t);

	json_t *p = 0;//(json_t *)calloc(1, sizeof(json_t));

	json_add(&p, "User", "Salih");
	json_add(&p, "Pass", "1/asd*af");
	json_add(&p, "Age", "31");
	json_add(&p, "Kg", "70");
	json_add(&p, "DArray", "[10,11,12]");
	json_add(&p, "CArray", "[\"abc\",\"def\",\"ghij\"]");
	json_add(&p, "Object", serialized);
	free(serialized);

	serialized = json_serialize(p);
	printf("Serialized %s\n", serialized);

	json_t *r = 0;
	json_add(&r, "Employe", serialized);

	free(serialized);
	serialized = json_serialize(r);
	printf("Serialized %s\n", serialized);

	json_remove(&p, "Age");

	free(serialized);
	serialized = json_serialize(p);
	printf("Serialized %s\n", serialized);

	//free(serialized);
	json_free(&p);
	json_free(&r);



	json_t *L = 0;
	json_deserialized(&L, serialized);
	free(serialized);

	printf("Read Kg %d\n", json_get_value(L, "Kg").d);

	char * user = json_get_value(L, "User").s;
	printf("Read User %s\n", user);
	free(user);

	char * pass = json_get_value(L, "Pass").s;
	printf("Read pass %s\n", pass);
	free(pass);

	printf("Read DArray \n");

	//array_t *a = json_parse_array("[\"12\",\"15\",\"16\"]");

	array_t *Array = json_get_value(L, "DArray").a;
	printf("Array Size %d, Type %c \n", Array->size, Array->type);

	if(Array->type == 'd')
		for(int i = 0; i < Array->size; i++)
			printf("array[%d]=%d\n", i, Array->array.Int[i]);

	json_free_array(&Array);

	Array = json_get_value(L, "CArray").a;
	printf("Array Size %d, Type %c \n", Array->size, Array->type);
	if(Array->type == 's')
		for(int i = 0; i < Array->size; i++)
			printf("array[%d]=%s\n", i, Array->array.Char[i]);

	json_free_array(&Array);

	printf("Read Object\n");
	json_t *Obj = json_get_value(L, "Object").o;
	char *a = json_get_value(Obj, "a").s;
	printf("Read Object for a %s\n", a);
	free(a);
	printf("Read Object for b %d\n", json_get_value(Obj, "b").d);

	json_free(&Obj);
	json_free(&L);
	return 0;
}
