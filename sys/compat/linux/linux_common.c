/*-
 * Copyright (c) 2014 Vassilis Laganakos
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/module.h>
#include <sys/malloc.h>
#include <sys/module.h>
#include <sys/types.h>
#include <sys/kernel.h>
#include <sys/proc.h>

#include <compat/linux/linux_mib.h>
#include <compat/linux/linux_util.h>

MODULE_VERSION(linux_common, 1);

SET_DECLARE(linux_device_handler_set, struct linux_device_handler);


static int
linux_common_modevent(module_t mod, int type, void *data)
{
	struct linux_device_handler **ldhp;

	switch(type) {
	case MOD_LOAD:
		linux_osd_jail_register();
		SET_FOREACH(ldhp, linux_device_handler_set)
			linux_device_register_handler(*ldhp);
		break;
	case MOD_UNLOAD:
		linux_osd_jail_deregister();
		SET_FOREACH(ldhp, linux_device_handler_set)
			linux_device_unregister_handler(*ldhp);
		break;
	default:
		return (EOPNOTSUPP);
	}
	return (0);
}

static moduledata_t linux_common_mod = {
	"linuxcommon",
	linux_common_modevent,
	0
};

DECLARE_MODULE(linuxcommon, linux_common_mod, SI_SUB_EXEC, SI_ORDER_ANY);
