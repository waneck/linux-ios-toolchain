/*
 * errno.h: wrapper linux </usr/include/errno.h>
 * */

#ifndef __ERRNO_H__ 
#define __ERRNO_H__  1

#include </usr/include/errno.h>

#if !defined(EBADRPC)
#	define	EBADRPC		72		/* RPC struct is bad */
#endif

#if !defined(EFTYPE)
#	define	EFTYPE		79		/* Inappropriate file type or format */
#endif

#endif /* __ERRNO_H__ */
