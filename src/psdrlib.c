/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#include <psdrlib.h>
#include <psdr-arch.h>

const unsigned int smp_nbits = BPS;

static int lib_idx = 0;
static struct psdrlib_t libs[PSDRLIB_MAX_INSTANCES];

struct psdrlib_t *psdrlib_init(void)
{
	struct psdrlib_t *lib;
	if (lib_idx == PSDRLIB_MAX_INSTANCES)
		return NULL;
	lib = &libs[lib_idx++];
	psdr_arch_init(lib);
	return lib;
}

void psdrlib_fini(struct psdrlib_t *lib)
{
	psdr_arch_fini(lib);
}

struct psdr_dev_t *psdr_enum_dev(struct psdr_dev_t *prev,
							   struct psdrlib_t *lib)
{
	/* FIXME find a way to enum on other buses too, if exist
	 * for this arch */
	return psdr_arch_usb_enum_dev(prev, lib);
}
