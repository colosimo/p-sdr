/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#ifndef _PSDRLIB_ARCH_API_H_
#define _PSDRLIB_ARCH_API_H_

#include <arch.h>

struct psdrlib_t;

/* Arch init and fini */
int psdr_arch_init(struct psdrlib_t *lib);
int psdr_arch_fini(struct psdrlib_t *lib);

/* Memory management functions */
void *psdr_alloc(unsigned size);
void psdr_free(void *ptr);

/* USB api required functions */
struct psdr_dev_t *psdr_arch_usb_enum_dev(struct psdr_dev_t *prev,
    struct psdrlib_t *lib);

#endif /* _PSDRLIB_ARCH_API_H_ */
