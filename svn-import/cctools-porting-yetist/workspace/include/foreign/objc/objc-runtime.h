/* vi: set sw=4 ts=4 wrap ai: */
/*
 * objc-runtime.h: This file is part of ____
 *
 * Copyright (C) 2012 yetist <xiaotian.wu@i-soft.com.cn>
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * */

#ifndef __OBJC_RUNTIME_H__ 
#define __OBJC_RUNTIME_H__  1

#include "objc/runtime.h"

#define CLS_GETINFO(cls,infomask)	((cls)->info & infomask)
#define CLS_SETINFO(cls,infomask)	((cls)->info |= infomask)

#define CLS_CLASS		0x1L
#define CLS_META		0x2L
#define CLS_INITIALIZED		0x4L
#define CLS_POSING		0x8L
#define CLS_MAPPED		0x10L
#define CLS_FLUSH_CACHE		0x20L
#define CLS_GROW_CACHE		0x40L
#define CLS_METHOD_ARRAY        0x100L

#endif /* __OBJC_RUNTIME_H__ */
