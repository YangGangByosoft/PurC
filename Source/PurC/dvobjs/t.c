/*
 * @file string.c
 * @author Geng Yue
 * @date 2021/07/02
 * @brief The implementation of string dynamic variant object.
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
#include "private/dvobjs.h"
#include "purc-variant.h"
#include "helper.h"

#include <limits.h>

static purc_variant_t
map_setter (purc_variant_t root, size_t nr_args, purc_variant_t *argv)
{
    UNUSED_PARAM(root);

    purc_variant_t ret_var = PURC_VARIANT_INVALID;
    size_t size = 0;

    if ((argv == NULL) || (nr_args < 2)) {
        pcinst_set_error (PURC_ERROR_WRONG_ARGS);
        return PURC_VARIANT_INVALID;
    }

    if ((argv[0] != PURC_VARIANT_INVALID) &&
            (!purc_variant_is_object (argv[0]))) {
        pcinst_set_error (PURC_ERROR_WRONG_ARGS);
        return PURC_VARIANT_INVALID;
    }
    purc_variant_object_size (argv[0], &size);

    if ((argv[1] != PURC_VARIANT_INVALID) &&
            (!purc_variant_is_string (argv[1]))) {
        pcinst_set_error (PURC_ERROR_WRONG_ARGS);
        return PURC_VARIANT_INVALID;
    }
    const char *option = purc_variant_get_string_const (argv[1]);
    size_t length = 0;
    purc_variant_string_bytes (argv[1], &length);

    pcintr_stack_t stack = purc_get_stack();
    if (stack && stack->dvobjs.t.dict == PURC_VARIANT_INVALID) {
        pcinst_set_error (PURC_ERROR_WRONG_ARGS);
        return PURC_VARIANT_INVALID;
    }

    switch (*option) {
        case 'a':
        case 'A':
        case 'd':
        case 'D':
            if ((strncasecmp (option, HVML_MAP_APPEND, length) == 0) ||
                    (strncasecmp (option, HVML_MAP_DISPLACE, length) == 0)) {
                struct purc_variant_object_iterator *it;
                it = purc_variant_object_make_iterator_begin(argv[0]);
                while (it) {
                    purc_variant_t key = purc_variant_object_iterator_get_key(it);
                    purc_variant_t val =
                        purc_variant_object_iterator_get_value(it);

                    purc_variant_object_set (stack->dvobjs.t.dict, key, val);

                    bool having = purc_variant_object_iterator_next(it);
                    if (!having) {
                        break;
                    }
                }
                if (it)
                    purc_variant_object_release_iterator(it);
            }
            break;
    }

    return ret_var;
}


static purc_variant_t
get_getter (purc_variant_t root, size_t nr_args, purc_variant_t *argv)
{
    UNUSED_PARAM(root);

    purc_variant_t ret_var = PURC_VARIANT_INVALID;

    if ((argv == NULL) || (nr_args < 1)) {
        pcinst_set_error (PURC_ERROR_WRONG_ARGS);
        return PURC_VARIANT_INVALID;
    }

    if ((argv[0] != PURC_VARIANT_INVALID) &&
            (!purc_variant_is_string (argv[0]))) {
        pcinst_set_error (PURC_ERROR_WRONG_ARGS);
        return PURC_VARIANT_INVALID;
    }

    pcintr_stack_t stack = purc_get_stack();
    if (stack && stack->dvobjs.t.dict == PURC_VARIANT_INVALID) {
        pcinst_set_error (PURC_ERROR_WRONG_ARGS);
        return PURC_VARIANT_INVALID;
    }

    ret_var = purc_variant_object_get (stack->dvobjs.t.dict, argv[0]);

    return ret_var;
}

purc_variant_t pcdvobjs_get_t (void)
{
    static struct pcdvobjs_dvobjs method [] = {
        {"map",  NULL,  map_setter},
        {"get",  get_getter, NULL},
    };

    return pcdvobjs_make_dvobjs (method, PCA_TABLESIZE(method));
}
