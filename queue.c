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
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q) {
        while (q->head != NULL) {
            list_ele_t *tmp = q->head;
            q->head = q->head->next;
            free(tmp->value);
            free(tmp);
        }
        free(q);
    }
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
    char *newc;
    newh = malloc(sizeof(list_ele_t));
    /* Deal with \0 in the tail of char * newc */
    if (newh == NULL) {
        return false;
    }
    newc = malloc(sizeof(char) * (strlen(s) + 1));
    if (newc == NULL) {
        free(newh);
        return false;
    }
    /* Situation that call to malloc returns NULL */
    strncpy(newc, s, strlen(s) + 1);
    newh->value = newc;
    newh->next = q->head;
    q->head = newh;
    /* Deal with new add data structures */
    if (q->size == 0) {
        q->tail = newh;
    }
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
    list_ele_t *newt;
    char *newc;
    newt = malloc(sizeof(list_ele_t));
    /* Deal with \0 in the tail of char * newc */
    if (newt == NULL) {
        return false;
    }
    newc = malloc(sizeof(char) * (strlen(s) + 1));
    if (newc == NULL) {
        free(newt);
        return false;
    }
    /* Situation that call to malloc returns NULL */
    strncpy(newc, s, strlen(s) + 1);
    newt->value = newc;
    if (q->head == NULL) {
        q->head = q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }
    q->tail->next = NULL;
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
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !q->head || !sp) {
        return false;
    }
    /*
    Because we can't not sure the cosistency of value and sp , we need to
    prevent overflow of sp
    */
    memset(sp, '\0', bufsize);
    strncpy(sp, q->head->value, bufsize - 1);
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);

    /* If q is empty */
    if (q->head) {
        q->tail = NULL;
    }

    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !q->head) {
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
    if (q && q->size > 0) {
        list_ele_t *current, *current_next;
        q->tail = current = q->head;
        current_next = current->next;
        while (current_next != NULL) {
            q->head = current_next;
            current_next = q->head->next;
            q->head->next = current;
            current = q->head;
        }
        q->tail->next = NULL;
    } else {
        return;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size <= 1) {
        return;
    }
    q->head = merge_sort_list(q->head);

    while (q->tail->next) {
        q->tail = q->tail->next;
    }
    return;
}


list_ele_t *merge_sort_list(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }
    list_ele_t *fast = head->next;
    list_ele_t *slow = head;
    /* spilt list */
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *l1 = merge_sort_list(head);
    list_ele_t *l2 = merge_sort_list(fast);
    return merge(l1, l2);
}

/*Merge sort usage, merge two short lists into a longer one*/

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t q;
    list_ele_t *temp = &q;
    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            temp->next = l1;
            temp = temp->next;
            l1 = l1->next;
        } else {
            temp->next = l2;
            temp = temp->next;
            l2 = l2->next;
        }
    }
    if (l1)
        temp->next = l1;
    if (l2)
        temp->next = l2;
    return q.next;
}

