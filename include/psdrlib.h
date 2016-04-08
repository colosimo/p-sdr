/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#ifndef _PSDRLIB_H_
#define _PSDRLIB_H_

#include <arch.h>
#include <psdr-devices.h>
#include <psdr-arch.h>

#ifndef psdr_printf
#define psdr_printf printf
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

#ifndef PSDRLIB_MAX_INSTANCES
#define PSDRLIB_MAX_INSTANCES 1
#endif

#include <arith.h>

struct psdrlib_t {
	void *arch_priv;
	void *priv;
};

struct psdrlib_t *psdrlib_init(void);

struct psdr_dev_t {
	enum psdr_dev_id id;
	void *priv;
};

/* Returns the pointer to a new psdr_dev_t.
 * Must be freed when no more used */
struct psdr_dev_t *psdr_enum_dev(struct psdr_dev_t *prev, struct psdrlib_t *lib);

#endif /* _PSDRLIB_H_ */
