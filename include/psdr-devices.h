/*
 * Author: Aurelio Colosimo, 2016
 * Released under MIT-Expat License.
 */

#ifndef _PPSDR_DEVICES_H_
#define _PPSDR_DEVICES_H_

enum {
	PSDR_RF_UNKNOWN = 0,
	PSDR_RF_E4000,
};

enum {
	PSDR_IQ_UNKNOWN,
	PSDR_IQ_RTL2832U = 1,
};

enum psdr_dev_id {
	PSDR_DEV_UNKNOWN,
	PSDR_DEV_RTL2832U_GENERIC,
	PSDR_DEV_EZCAP_DVB_DONGLE,
};

static const char *psdr_dev_str[] = {
	"Unknown",
	"Generic RTL2832U",
	"Ezcap DVB-T/DAB/FM dongle",
};

static inline const char *dev_id_str(enum psdr_dev_id id) {
	return psdr_dev_str[id];
}

#define DECLARE_PSDR_DEV(d, iq, r) \
	{.dev_id = d, .iq_id = iq, .rf_id = r}

struct psdr_supported_dev_t {
	enum psdr_dev_id dev_id;
	unsigned iq_id;
	unsigned rf_id;
};

static struct psdr_supported_dev_t supported_devices[] = {
	DECLARE_PSDR_DEV(PSDR_DEV_RTL2832U_GENERIC, PSDR_IQ_RTL2832U, PSDR_RF_E4000),
	DECLARE_PSDR_DEV(PSDR_DEV_EZCAP_DVB_DONGLE, PSDR_IQ_RTL2832U, PSDR_RF_E4000),
};

#endif /* _PSDR_DEVICES_H_ */
