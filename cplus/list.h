/*******************************************************************************
 * Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.
 * File name: list.h
 * Author:
 * Version:
 * Date: 2017-01-24
 * Description: link list module
 *******************************************************************************/

#ifndef __LIST_H__
#define __LIST_H__

#include <unistd.h>  // for NULL pointer definition
#include "lint_check.h"

static inline void prefetch(void *x) {
    asm volatile("prefetcht0 %0" ::"m"(*(unsigned long *)x));
}

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct ListHead {
    struct ListHead *next, *prev;
};

#define LT_HEAD_INIT(name) \
    { &(name), &(name) }
#define LT_HEAD(name) \
    { struct ListHead name = LT_HEAD_INIT(name) }

#define INIT_LT_HEAD(ptr)    \
    do {                     \
        (ptr)->next = (ptr); \
        (ptr)->prev = (ptr); \
    } while (0)

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void _ListAdd(struct ListHead *new_node, struct ListHead *prev,
                            struct ListHead *next) {
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

/**
 * ListAdd - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void ListAdd(struct ListHead *new_node, struct ListHead *head) {
    _ListAdd(new_node, head, head->next);
}

/**
 * ListAddTail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void ListAddTail(struct ListHead *new_node,
                               struct ListHead *head) {
    _ListAdd(new_node, head->prev, head);
}

static inline void ListAddPrev(struct ListHead *cur, struct ListHead *node) {
    _ListAdd(node, cur->prev, cur);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void _ListDel(struct ListHead *prev, struct ListHead *next) {
    next->prev = prev;
    prev->next = next;
}

/**
 * ListDel - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: ListEmpty on entry does not return true after this, the entry is in an
 * undefined state.
 */
// for so called symbol overlay
/*lint -e{578}*/
static inline void ListDel(struct ListHead *entry) {
    _ListDel(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}
/**
 * ListDelInit - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
/*lint -e{578}*/
static inline void ListDelInit(struct ListHead *entry) {
    _ListDel(entry->prev, entry->next);
    INIT_LT_HEAD(entry);
}

/**
 * ListEmpty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int ListEmpty(struct ListHead *head) {
    return head->next == head;
}

static inline void __list_splice(struct ListHead *_list,
                                 struct ListHead *head) {
    struct ListHead *first = _list->next;
    struct ListHead *last = _list->prev;
    struct ListHead *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static inline void list_splice(struct ListHead *_list, struct ListHead *head) {
    if (!ListEmpty(_list)) {
        __list_splice(_list, head);
    }
}

/**
 * list_splice_init - join two lists and reinitialise the emptied list.
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 *
 * The list at @list is reinitialised
 */
static inline void list_splice_init(struct ListHead *_list,
                                    struct ListHead *head) {
    if (!ListEmpty(_list)) {
        __list_splice(_list, head);
        INIT_LT_HEAD(_list);
    }
}

static inline void list_replace(struct ListHead *old, struct ListHead *_new) {
    _new->next = old->next;
    _new->next->prev = _new;
    _new->prev = old->prev;
    _new->prev->next = _new;
}

static inline void list_replace_init(struct ListHead *old,
                                     struct ListHead *_new) {
    list_replace(old, _new);
    INIT_LT_HEAD(old);
}

/**
* list_del_init - deletes entry from list and reinitialize it.
* @entry: the element to delete from the list.
*/
static inline void list_del_init(struct ListHead *entry) {
    ListDel(entry);
    INIT_LT_HEAD(entry);
}

/**
* list_move - delete from one list and add as another's head
* @list: the entry to move
* @head: the head that will precede our entry
*/
static inline void list_move(struct ListHead *list, struct ListHead *head) {
    ListDel(list);
    ListAdd(list, head);
}

static inline struct ListHead *list_get(struct ListHead *head)  // lint !e528
{
    // struct ListHead *list = NULL;
    if (ListEmpty(head)) {
        return NULL;
    } else {
        return head->next;
    }
}

/**
 * lt_entry - get the struct for this entry
 * @ptr:    the &struct ListHead pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 */
#define lt_entry(ptr, type, member) \
    /*lint -save -e413*/            \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
/*lint -restore*/

#define first_lt_entry(head, type, member) lt_entry((head)->next, type, member)

/**
 * lt_for_each_reversed    -   iterate over a list
 * @pos:    the &struct ListHead to use as a loop counter.
 * @head:   the head for your list.
 */
#define lt_for_each_reversed(pos, head)                          \
    for (pos = (head)->prev, prefetch(pos->prev); pos != (head); \
         pos = pos->prev, prefetch(pos->prev))

/**
 * lt_for_each_safe_reversed   -   iterate over a list safe against removal of
 * list entry
 * @pos:    the &struct ListHead to use as a loop counter.
 * @n:      another &struct ListHead to use as temporary storage
 * @head:   the head for your list.
 */
#define lt_for_each_safe_reversed(pos, n, head)            \
    for (pos = (head)->prev, n = pos->prev; pos != (head); \
         pos = n, n = pos->prev)

/**
 * lt_for_each    -   iterate over a list
 * @pos:    the &struct ListHead to use as a loop counter.
 * @head:   the head for your list.
 */
#define lt_for_each(pos, head)                                   \
    for (pos = (head)->next, prefetch(pos->next); pos != (head); \
         pos = pos->next, prefetch(pos->next))

/**
 * lt_for_each_safe   -   iterate over a list safe against removal of list entry
 * @pos:    the &struct ListHead to use as a loop counter.
 * @n:      another &struct ListHead to use as temporary storage
 * @head:   the head for your list.
 */
#define lt_for_each_safe(pos, n, head)                     \
    for (pos = (head)->next, n = pos->next; pos != (head); \
         pos = n, n = pos->next)

#define lt_for_each_entry(pos, head, member)                 \
    for (pos = lt_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head);                             \
         pos = lt_entry(pos->member.next, typeof(*pos), member))

/**
 * lt_for_each_entry_safe - iterate over list of given type safe against removal
 * of list entry
 * @pos:        the type * to use as a loop counter.
 * @n:          another type * to use as temporary storage
 * @head:       the head for your list.
 * @member:     the name of the list_struct within the struct.
 */
/*begin by jiahengsong j00128576 for pclint warning 613 20080725*/
#define lt_for_each_entry_safe(pos, n, head, member)          \
    /*lint -e40 -e613 -e48 -e10 -e64 -e666 -e144*/            \
    for (pos = lt_entry((head)->next, typeof(*pos), member),  \
        n = lt_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head);                              \
         pos = n, n = lt_entry(n->member.next, typeof(*n), member))
/*lint +e40 +e613 +e48 +e10 +e64 +e666 +e144*/
/*begin by jiahengsong j00128576 for pclint warning 613 20080725*/

#endif
