/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#include <psdrlib.h>

int main(int argc, char **argv)
{
	struct psdrlib_t *lib;
	struct psdr_dev_t *d = NULL;
	unsigned c = 0;

	lib = psdrlib_init();

	if (!lib)
		return 1;

	/* Example of loop enumerating all found devices */
	while (d = psdr_enum_dev(d, lib)) {
		c++;
		psdr_printf("Found device #%u: %s\n", c, dev_id_str(d->id));
	}

	if (!c) {
		psdr_printf("No supported psdr devices found\n");
		return 2;
	}

	psdr_free(d);
	psdrlib_fini(lib);
}
