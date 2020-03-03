#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;
    }
    q->size = 0;
    q->head = NULL;
    q->tail = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL) {
        return;
    }
    while (q->head) {
        list_ele_t *head = q->head;
        q->head = q->head->next;
        if (head->value != NULL) {
            free(head->value);
        }
        free(head);
    }
    /* Free queue structure */
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
    if (q == NULL) {
        return false;
    }
    list_ele_t *newh;
    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    }
    newh->value = (char *) malloc(strlen(s) + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    newh->next = q->head;
    if (q->size == 0) {
        q->tail = newh;
    }
    q->head = newh;
    q->size++;
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
    if (q == NULL) {
        return false;
    }
    /* Remember: It should operate in O(1) time */
    list_ele_t *newh;
    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    }
    newh->value = (char *) malloc(strlen(s) + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    newh->next = NULL;
    if (q->size == 0) {
        q->tail = q->head = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    q->size++;
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
    if (q_size(q) == 0) {
        return false;
    }
    if (sp != NULL) {
        strncpy(sp, q->head->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    list_ele_t *del = q->head;
    q->head = q->head->next;
    if (del->value != NULL) {
        free(del->value);
    }
    free(del);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL) {
        return 0;
    }
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
    if (q_size(q) == 0) {
        return;
    }
    list_ele_t *cur = q->head;
    list_ele_t *next = cur->next;
    q->tail = cur;
    cur->next = NULL;
    while (next != NULL) {
        list_ele_t *tmp = next->next;
        next->next = cur;
        cur = next;
        next = tmp;
    }
    q->head = cur;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
int cmp(char *s1, char *s2)
{
    return strcmp(s1, s2);
}

list_ele_t *merge_sort(list_ele_t *start, int Ln, int Rn)
{
    if (Rn <= Ln) {
        return start;
    }
    int M = (Ln + Rn) / 2;
    list_ele_t *mid = start;
    list_ele_t *prev = start;
    for (int i = Ln; i <= M; i++) {
        prev = mid;
        mid = mid->next;
    }
    prev->next = NULL;
    start = merge_sort(start, Ln, M);
    mid = merge_sort(mid, M + 1, Rn);
    list_ele_t *merged = NULL;
    list_ele_t *head;
    if (cmp(start->value, mid->value) > 0) {
        merged = mid;
        mid = mid->next;
    } else {
        merged = start;
        start = start->next;
    }
    head = merged;
    while (start && mid) {
        if (cmp(start->value, mid->value) > 0) {
            merged->next = mid;
            merged = merged->next;
            mid = mid->next;
        } else {
            merged->next = start;
            merged = merged->next;
            start = start->next;
        }
    }

    if (start) {
        merged->next = start;
    }
    if (mid) {
        merged->next = mid;
    }

    return head;
}



void q_sort(queue_t *q)
{
    if (q_size(q) == 0) {
        return;
    }
    list_ele_t *node = merge_sort(q->head, 0, q->size - 1);
    q->head = node;
    for (int i = 0; i < q->size - 1; i++) {
        node = node->next;
    }
    q->tail = node;
}
