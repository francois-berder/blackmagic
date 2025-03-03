/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2015  Black Sphere Technologies Ltd.
 * Written by Gareth McMullin <gareth@blacksphere.co.nz>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "general.h"
#include "exception.h"

exception_s *innermost_exception = NULL;

void raise_exception(const uint32_t type, const char *const msg)
{
	for (exception_s *exception = innermost_exception; exception; exception = exception->outer) {
		if (exception->mask & type) {
			exception->type = type;
			exception->msg = msg;
			innermost_exception = exception->outer;
			longjmp(exception->jmpbuf, type);
		}
	}
	DEBUG_WARN("Unhandled exception: %s\n", msg);
	abort();
}
