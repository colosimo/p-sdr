/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <arch.h>
#include <psdrlib.h>
#include <libusb.h>

#define lib_to_arch(x) ((struct unix_arch_t *)x->arch_priv)
#define lib_to_usbctx(x) lib_to_arch(x)->ctx

struct unix_arch_t {
	libusb_context *ctx;
	libusb_device **devs;
	int ndevs;
	int it;
};

struct unix_devid_t {
	uint16_t vid;
	uint16_t pid;
	enum psdr_dev_id dev_id;
};

extern const struct psdr_drv_t rtl2832u_drv;

static struct unix_devid_t dev_ids[] = {
	{ .vid = 0x0bda, .pid = 0x2832, .dev_id = PSDR_DEV_RTL2832U_GENERIC},
	{ .vid = 0x0bda, .pid = 0x2838, .dev_id = PSDR_DEV_EZCAP_DVB_DONGLE},
};

static int uarch_idx = 0;
static struct unix_arch_t uarch[PSDRLIB_MAX_INSTANCES];

static inline int psdr_find_dev(uint16_t v, uint16_t p)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(dev_ids); i++) {
		if (dev_ids[i].vid == v && dev_ids[i].pid == p)
			return i;
	}
	return -1;
}

int psdr_arch_init(struct psdrlib_t *lib)
{
	if (uarch_idx == PSDRLIB_MAX_INSTANCES)
		return -EINVAL;
	lib->arch_priv = &uarch[uarch_idx];
	libusb_init(&uarch[uarch_idx].ctx);
	libusb_set_debug(uarch[uarch_idx].ctx, 3);
	uarch_idx++;
	return 0;
}

int psdr_arch_fini(struct psdrlib_t *lib)
{
	libusb_context *ctx = lib_to_usbctx(lib);
	libusb_exit(ctx);
	return 0;
}

void *psdr_alloc(unsigned size)
{
	return calloc(size, 1);
}

void psdr_free(void *ptr)
{
	free(ptr);
}

void *psdr_memcpy(void *dest, const void *src, unsigned n)
{
	return memcpy(dest, src, n);
}

struct psdr_dev_t *psdr_arch_usb_enum_dev(struct psdr_dev_t *prev,
    struct psdrlib_t *lib)
{
	struct unix_arch_t *a = lib_to_arch(lib);
	libusb_context *ctx = a->ctx;
	struct libusb_device_descriptor d;
	struct psdr_dev_t *dev;
	int i;

	if (prev == NULL) {
		a->ndevs = libusb_get_device_list(ctx, &a->devs);
		a->it = 0;
	}

	for (; a->it < a->ndevs; a->it++) {
		libusb_get_device_descriptor(a->devs[a->it], &d);
		if ((i = psdr_find_dev(d.idVendor, d.idProduct)) >= 0) {
			dev = psdr_alloc(sizeof(*dev));

			/* dev->priv is assigned the corresponding libusb_device ptr */
			dev->priv = a->devs[a->it];
			dev->id = dev_ids[i].dev_id;
			a->it++;
			return dev;
		}
	}
	return NULL;
}

/* You can call intp2str or uintp2str up to 16 times in the same sdr_printf */
#if INTP == int16_t
static char tmp[16][8];
#elif INTP == int32_t
static char tmp[16][12];
#elif INTP == int64_t
static char tmp[16][22];
#endif

static unsigned i;
const char *intp2str(intp x)
{
	i = (i + 1) % 16;
#if INTP == int16_t
	sprintf(tmp[i], "%hd", (uint16_t)x);
#elif INTP == int32_t
	sprintf(tmp[i], "%ld", (int32_t)x);
#elif INTP == int64_t
	sprintf(tmp[i], "%ll", (int64_t)x);
#endif
	return tmp[i];
}

const char *uintp2str(intp x)
{
	i = (i + 1) % 16;
#if UINTP == uint16_t
	sprintf(tmp[i], "%hu", (uint16_t)x);
#elif UINTP == uint32_t
	sprintf(tmp[i], "%lu", (uint32_t)x);
#elif UINTP == uint64_t
	sprintf(tmp[i], "%llu", (uint64_t)x);
#endif
	return tmp[i];
}
