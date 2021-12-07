/**
 * @file interpreter.c
 * @author Xu Xiaohong
 * @date 2021/11/18
 * @brief The internal interfaces for interpreter
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
 *
 */

#include "config.h"

#include "private/debug.h"
#include "private/instance.h"
#include "private/interpreter.h"

#include "iterate.h"

void pcintr_stack_init_once(void)
{
}

void pcintr_stack_init_instance(struct pcinst* inst)
{
    struct pcintr_stack *intr_stack = &inst->intr_stack;
    memset(intr_stack, 0, sizeof(*intr_stack));

    struct list_head *frames = &intr_stack->frames;
    INIT_LIST_HEAD(frames);
    intr_stack->ret_var = PURC_VARIANT_INVALID;
}

static inline void
intr_stack_frame_release(struct pcintr_stack_frame *frame)
{
    frame->scope = NULL;
    frame->pos   = NULL;

    frame->ctxt  = NULL;

    for (size_t i=0; i<PCA_TABLESIZE(frame->symbol_vars); ++i) {
        PURC_VARIANT_SAFE_CLEAR(frame->symbol_vars[i]);
    }

    PURC_VARIANT_SAFE_CLEAR(frame->attr_vars);

    PURC_VARIANT_SAFE_CLEAR(frame->ctnt_var);

    PURC_VARIANT_SAFE_CLEAR(frame->mid_vars);
}

void pcintr_stack_cleanup_instance(struct pcinst* inst)
{
    struct pcintr_stack *intr_stack = &inst->intr_stack;
    struct list_head *frames = &intr_stack->frames;
    if (!list_empty(frames)) {
        struct pcintr_stack_frame *p, *n;
        list_for_each_entry_safe(p, n, frames, node) {
            list_del(&p->node);
            --intr_stack->nr_frames;
            intr_stack_frame_release(p);
            free(p);
        }
        PC_ASSERT(intr_stack->nr_frames == 0);
    }
}

pcintr_stack_t purc_get_stack(void)
{
    struct pcinst *inst = pcinst_current();
    struct pcintr_stack *intr_stack = &inst->intr_stack;
    return intr_stack;
}

struct pcintr_stack_frame*
pcintr_stack_get_bottom_frame(pcintr_stack_t stack)
{
    if (!stack)
        return NULL;

    if (stack->nr_frames < 1)
        return NULL;

    struct list_head *tail = stack->frames.prev;
    return container_of(tail, struct pcintr_stack_frame, node);
}

static inline struct pcintr_stack_frame*
push_stack_frame(pcintr_stack_t stack)
{
    PC_ASSERT(stack);
    struct pcintr_stack_frame *frame;
    frame = (struct pcintr_stack_frame*)calloc(1, sizeof(*frame));
    if (!frame)
        return NULL;

    list_add_tail(&frame->node, &stack->frames);
    ++stack->nr_frames;

    return 0;
}

static inline void
pop_stack_frame(pcintr_stack_t stack)
{
    PC_ASSERT(stack);
    PC_ASSERT(stack->nr_frames > 0);

    struct list_head *tail = stack->frames.prev;
    PC_ASSERT(tail != NULL);
    PC_ASSERT(tail != &stack->frames);

    list_del(tail);

    struct pcintr_stack_frame *frame;
    frame = container_of(tail, struct pcintr_stack_frame, node);

    intr_stack_frame_release(frame);
    free(frame);
}

struct pcintr_element_ops*
pcintr_get_element_ops(pcvdom_element_t element)
{
    PC_ASSERT(element);

    switch (element->tag_id) {
        case PCHVML_TAG_ITERATE:
            return pcintr_iterate_get_ops();
        default:
            return NULL;
    }
}

static inline int
element_post_load(struct pcvdom_document *document,
        struct pcvdom_element *element)
{
    UNUSED_PARAM(document);
    UNUSED_PARAM(element);
    PC_ASSERT(0); // Not implemented yet
    return -1;
}

static inline int
document_post_load(struct pcvdom_document *document)
{
    struct pcvdom_node *node = &document->node;
    struct pcvdom_node *p = pcvdom_node_first_child(node);
    (void)p;
    PC_ASSERT(0); // Not implemented yet

    struct pcvdom_element *element;
    int r = 0;
    for (; p; p = pcvdom_node_next_sibling(p)) {
        switch (p->type) {
            case PCVDOM_NODE_DOCUMENT:
                PC_ASSERT(0);
                return -1;
            case PCVDOM_NODE_ELEMENT:
                element = container_of(p, struct pcvdom_element, node);
                r = element_post_load(document, element);
                break;
            case PCVDOM_NODE_CONTENT:
                // FIXME: output to edom?
                break;
            case PCVDOM_NODE_COMMENT:
                // FIXME:
                break;
            default:
                PC_ASSERT(0);
                return -1;
        }
        if (r)
            return -1;
    }

    return -1;
}

int pcintr_post_load(purc_vdom_t vdom)
{
    PC_ASSERT(vdom);
    pcintr_stack_t stack = purc_get_stack();
    PC_ASSERT(stack->nr_frames == 0);

    struct pcintr_stack_frame *frame;
    frame = push_stack_frame(stack);
    if (!frame)
        return -1;
    struct pcvdom_document *document = vdom->document;
    int r = document_post_load(document);
    PC_ASSERT(stack->nr_frames == 1);
    pop_stack_frame(stack);

    return r ? -1 : 0;
}

static inline bool
set_object_by(purc_variant_t obj, struct pcintr_dynamic_args *arg)
{
    purc_variant_t dynamic;
    dynamic = purc_variant_make_dynamic(arg->getter, arg->setter);
    if (dynamic == PURC_VARIANT_INVALID)
        return false;

    bool ok = purc_variant_object_set_by_static_ckey(obj, arg->name, dynamic);
    if (!ok) {
        purc_variant_unref(dynamic);
        return false;
    }

    return true;
}

purc_variant_t
pcintr_make_object_of_dynamic_variants(size_t nr_args,
    struct pcintr_dynamic_args *args)
{
    purc_variant_t obj;
    obj = purc_variant_make_object_by_static_ckey(0,
            NULL, PURC_VARIANT_INVALID);

    if (obj == PURC_VARIANT_INVALID)
        return PURC_VARIANT_INVALID;

    for (size_t i=0; i<nr_args; ++i) {
        struct pcintr_dynamic_args *arg = args + i;
        if (!set_object_by(obj, arg)) {
            purc_variant_unref(obj);
            return false;
        }
    }

    return obj;
}


