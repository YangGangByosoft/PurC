/*
 * @file dvobjs.c
 * @author Geng Yue
 * @date 2021/07/02
 * @brief The interface of dynamic variant objects.
 *
 * Copyright (C) 2021 FMSoft <https://www.fmsoft.cn>
 *
 * This file is a part of PurC (short for Purring Cat), an HVML interpreter.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "private/instance.h"
#include "private/errors.h"
#include "private/interpreter.h"
#include "private/dvobjs.h"
#include "purc-variant.h"

#include <time.h>
#include <limits.h>

void pcdvobjs_init_once(void)
{
    // initialize others
}

void pcdvobjs_init_instance(struct pcinst* inst)
{
    UNUSED_PARAM(inst);
    srand(time(NULL));
}

void pcdvobjs_cleanup_instance(struct pcinst* inst)
{
    UNUSED_PARAM(inst);
}

void dvobjs_init (pcintr_stack_t stack)
{
    if (stack) {
        stack->dvobjs.hvml.url = NULL;
        stack->dvobjs.hvml.maxIterationCount = ULONG_MAX;
        stack->dvobjs.hvml.maxRecursionDepth = USHRT_MAX;

        stack->dvobjs.t.dict = purc_variant_make_object (0,
                PURC_VARIANT_INVALID, PURC_VARIANT_INVALID);
    }
}

void dvobjs_release (pcintr_stack_t stack)
{
    if (stack) {
        if (stack->dvobjs.hvml.url)
            free (stack->dvobjs.hvml.url);

        purc_variant_unref (stack->dvobjs.t.dict);
    }
}
