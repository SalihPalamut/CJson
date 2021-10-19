#include <stdio.h>
#include <stdlib.h>
#include "json.h"

int main()
{
	char * serialized = 0;
	json_t *p = 0;//(json_t *)calloc(1, sizeof(json_t));

	json_add(&p, "User", "Salih");
	json_add(&p, "Pass", "1/asd*af");
	json_add(&p, "Age", "31");

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

	free(serialized);
	json_free(&p);
	json_free(&r);
	return 0;
}
