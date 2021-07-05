/**
 * @file purc.h
 * @author Vincent Wei (https://github.com/VincentWei)
 * @date 2021/07/02
 * @brief The main header file of PurC.
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

#ifndef PURC_PURC_H
#define PURC_PURC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "purc-macros.h"
#include "purc-version.h"
#include "purc-errors.h"
#include "purc-variant.h"
#include "purc-rwstream.h"

typedef struct purc_instance_extra_info {
    int foo;
    int bar;
} purc_instance_extra_info;

typedef struct purc_rdr_extra_info {
    int foo;
    int bar;
} purc_rdr_extra_info;

PCA_EXTERN_C_BEGIN

/**
 * purc_init:
 *
 * @app_name: a pointer to the string contains the app name.
 *      If this argument is null, the executable program name of the command line
 *      will be used for the app name.
 * @runner_name: a pointer to the string contains the runner name.
 *      If this argument is null, `unknown` will be used for the runner name.
 * @extra_info: a pointer (nullable) to the extra information for
 *      the new PurC instance.
 *
 * Initializes a new PurC instance for the current thread.
 *
 * Returns: the error code:
 *  - @PURC_ERROR_OK: success
 *  - @PURC_ERROR_DUPLICATED: duplicated call of this function.
 *  - @PURC_ERROR_OUT_OF_MEMORY: Out of memory.
 *
 * Note that this function is the only one which returns the error code directly.
 * Because if it fails, there is no any space to store the error code.
 *
 * Since 0.0.1
 */
PCA_EXPORT int
purc_init(const char* app_name, const char* runner_name,
        const purc_instance_extra_info* extra_info);

/**
 * purc_cleanup:
 *
 * Cleans up the PurC instance attached to the current thread.
 *
 * Returns: @true for success; @false for no PurC instance for
 *      the current thread.
 *
 * Since 0.0.1
 */
PCA_EXPORT bool
purc_cleanup(void);

/**
 * purc_bind_session_variable:
 *
 * @name: The pointer to the string contains the name for the variable.
 * @variant: The variant.
 *
 * Binds a variant value as the session-level variable.
 *
 * Returns: @true for success; @false for failure.
 *
 * Since 0.0.1
 */
PCA_EXPORT bool
purc_bind_session_variable(const char* name, purc_variant_t variant);

struct pcvdom_tree;
typedef struct pcvdom_tree  pcvdom_tree;
typedef struct pcvdom_tree* pcvdom_tree_t;

/**
 * purc_load_hvml_from_string:
 *
 * @string: The pointer to the string contains the HVML docment.
 *
 * Loads a HVML program from a string.
 *
 * Returns: A valid pointer to the vDOM tree for success; @NULL for failure.
 *
 * Since 0.0.1
 */
PCA_EXPORT pcvdom_tree_t
purc_load_hvml_from_string(const char* string);

/**
 * purc_load_hvml_from_file:
 *
 * @file: The pointer to the string contains the file name.
 *
 * Loads a HVML program from a file.
 *
 * Returns: A valid pointer to the vDOM tree for success; @NULL for failure.
 *
 * Since 0.0.1
 */
PCA_EXPORT pcvdom_tree_t
purc_load_hvml_from_file(const char* file);

/**
 * purc_load_hvml_from_url:
 *
 * @url: The pointer to the string contains the URL.
 *
 * Loads a HVML program from the speicifed URL.
 *
 * Returns: A valid pointer to the vDOM tree for success; @NULL for failure.
 *
 * Since 0.0.1
 */
PCA_EXPORT pcvdom_tree_t
purc_load_hvml_from_url(const char* url);

/**
 * purc_load_hvml_from_rwstream:
 *
 * @stream: The RWSTream object.
 *
 * Loads a HVML program from the specified RWStream object.
 *
 * Returns: A valid pointer to the vDOM tree for success; @NULL for failure.
 *
 * Since 0.0.1
 */
PCA_EXPORT pcvdom_tree_t
purc_load_hvml_from_rwstream(purc_rwstream_t stream);

/**
 * purc_bind_document_variable:
 *
 * @name: The pointer to the string contains the name for the variable.
 * @variant: The variant.
 *
 * Binds a variant value as the document-level variable of the specified vDOM.
 *
 * Returns: @true for success; @false for failure.
 *
 * Since 0.0.1
 */
PCA_EXPORT bool
purc_bind_document_variable(pcvdom_tree_t vdom, const char* name, purc_variant_t variant);

/**
 * purc_connnect_vdom_to_renderer:
 *
 * @vdom: The pointer to the string contains the name for the variable.
 * @type: The pointer to the string contains the type of the expected renderer.
 * @classes: The pointer to the string contains the classes of the expected renderer.
 * @name: The pointer to the string contains the name of the expected renderer.
 * @extra_info: The structure pointer to the extra information of the expected renderer.
 *
 * Connects a vDOM tree to an renderer.
 *
 * Returns: @true for success; @false for failure.
 *
 * Since 0.0.1
 */
PCA_EXPORT bool
purc_connnect_vdom_to_renderer(pcvdom_tree_t vdom,
        const char* type, const char* name,
        const purc_rdr_extra_info* extra_info);

typedef int (*purc_event_handler)(pcvdom_tree_t vdom, purc_variant_t event);

/**
 * purc_run:
 *
 * @request: The variant which will be used for request data.
 * @handler: The pointer to a call-back function which handles
 *      the session events.
 *
 * Runs all HVML programs which has connected to a renderer in
 * the current PurC instance.
 *
 * Returns: @true for success; @false for failure.
 *
 * Since 0.0.1
 */
PCA_EXPORT bool
purc_run(purc_variant_t request, purc_event_handler handler);

PCA_EXTERN_C_END

#endif /* not defined PURC_PURC_H */
