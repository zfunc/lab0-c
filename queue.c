#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

static bool compare(char *, char *);
static list_ele_t *merge_sort(list_ele_t *, int);
static list_ele_t *merge(list_ele_t *, list_ele_t *, int);
/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }
    list_ele_t *cur = q->head;
    while (cur) {
        list_ele_t *tmp = cur->next;
        free(cur->value);
        free(cur);
        cur = tmp;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    if (!q) {
        return false;
    }
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    newh->next = q->head;
    newh->value = malloc(strlen(s) + 1);
    if (!(newh->value)) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    if (q->size == 0)
        q->tail = newh;
    q->head = newh;
    q->size = q->size + 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = malloc(strlen(s) + 1);
    if (!(newt->value)) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, strlen(s) + 1);
    newt->next = NULL;
    if (!(q->tail)) {
        q->head = newt;
    } else {
        q->tail->next = newt;
    }
    q->tail = newt;
    q->size += 1;
    return true;
}
/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !(q->head))
        return false;
    list_ele_t *old = q->head;
    if (sp) {
        strncpy(sp, old->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    q->head = q->head->next;
    q->size -= 1;

    free(old->value);
    free(old);

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !(q->head))
        return 0;
    else
        return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    list_ele_t *cur = q->head;
    list_ele_t *next = cur->next;
    list_ele_t *nnext;
    if (next) {
        nnext = next->next;
    } else {
        nnext = NULL;
    }
    while (next) {
        next->next = cur;
        cur = next;
        next = nnext;
        if (next) {
            nnext = next->next;
        } else {
            nnext = NULL;
        }
    }
    q->tail = q->head;
    q->tail->next = NULL;
    q->head = cur;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */



void q_sort(queue_t *q)
{
    if (!q || !(q->head) || !(q->head->next))
        return;
    q->head = merge_sort(q->head, q->size);
    list_ele_t *tail = q->tail;
    for (; tail->next != NULL; tail = tail->next)
        ;
    q->tail = tail;
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}

static list_ele_t *merge(list_ele_t *l, list_ele_t *r, int size)
{
    list_ele_t *idx = NULL, *head = NULL;
    while (size--) {
        if (!r || (l && compare(l->value, r->value))) {
            if (idx == NULL) {
                head = idx = l;
            } else {
                idx->next = l;
                idx = idx->next;
            }
            l = l->next;
        } else {
            if (idx == NULL) {
                head = idx = r;
            } else {
                idx->next = r;
                idx = idx->next;
            }
            r = r->next;
        }
    }
    return head;
}

static list_ele_t *merge_sort(list_ele_t *l, int size)
{
    if (!l || size <= 1) {
        return l;
    }
    list_ele_t *r = l;
    int i;
    for (i = 0; i < size / 2; i++)
        r = r->next;
    if (l == r)
        return l;

    list_ele_t *ll = merge_sort(l, size / 2);
    list_ele_t *tmp = ll;
    for (i = 0; i < size / 2 - 1; i++)
        tmp = tmp->next;
    tmp->next = NULL;
    list_ele_t *rl = merge_sort(r, (size + 1) / 2);
    tmp = rl;
    for (i = 0; i < (size + 1) / 2 - 1; i++)
        tmp = tmp->next;
    tmp->next = NULL;
    list_ele_t *merged = merge(ll, rl, size);
    return merged;
}

static bool compare(char *a, char *b)
{
    int i, j;
    unsigned lena = strlen(a);
    unsigned lenb = strlen(b);

    for (i = 0, j = 0; i < lena && j < lenb; i++, j++) {
        if (a[i] != b[j])
            return a[i] < b[j];
    }

    return lena <= lenb;
}
