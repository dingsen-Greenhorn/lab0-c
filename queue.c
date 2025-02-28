#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_queue =
        (struct list_head *) malloc(sizeof(struct list_head));

    if (!new_queue) {
        return NULL;
    }

    INIT_LIST_HEAD(new_queue);
    return new_queue;
}


/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;
    }

    struct list_head *node, *safe_front;

    list_for_each_safe (node, safe_front, head) {
        list_del(node);
        free(list_entry(node, element_t, list)->value);
        free(list_entry(node, element_t, list));
    }
    free(head);
}


/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *new_content = malloc(sizeof(element_t));

    if (!new_content) {
        return false;
    }

    INIT_LIST_HEAD(&new_content->list);
    new_content->value = strdup(s);

    if (!new_content->value) {
        free(new_content);
        return false;
    }

    list_add(&new_content->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *new_content = malloc(sizeof(element_t));

    if (!new_content) {
        return false;
    }
    INIT_LIST_HEAD(&new_content->list);
    new_content->value = strdup(s);

    if (!new_content->value) {
        free(new_content);
        return false;
    }

    list_add_tail(&new_content->list, head);
    return true;
}

/* Remove an element at head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *first = list_first_entry(head, element_t, list);
    list_del(&first->list);

    if (sp && bufsize > 0) {
        strncpy(sp, first->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return first;
}


/* Remove an element at tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *tail = list_last_entry(head, element_t, list);
    list_del(&tail->list);

    if (sp && bufsize > 0) {
        strncpy(sp, tail->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return tail;
}


/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *lenptr;

    list_for_each (lenptr, head)
        len = len + 1;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *fast = head->next, *slow = head->next;

    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }
    element_t *mid = list_entry(slow, element_t, list);
    list_del(slow);
    free(mid->value);
    free(mid);
    return true;
}


/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return false;
    }

    struct list_head *node, *safe;
    element_t *prev = NULL, *curr = NULL;
    bool has_dup = false;

    list_for_each_safe (node, safe, head) {
        curr = list_entry(node, element_t, list);

        if (prev && !strcmp(prev->value, curr->value)) {
            list_del(node);
            free(curr->value);
            free(curr);
            has_dup = true;
        } else {
            if (has_dup) {
                list_del(&prev->list);
                free(prev->value);
                free(prev);
                has_dup = false;
            }
            prev = curr;
        }
    }
    if (has_dup && prev) {
        list_del(&prev->list);
        free(prev->value);
        free(prev);
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    q_reverseK(head, 2);
}

/* Reverse elements in queue */
/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        node->next = node->prev;
        node->prev = safe;
    }
    node->next = node->prev;
    node->prev = safe;
    return;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head) {
        return;
    }

    int split_time = q_size(head) / k;
    struct list_head *tail;

    LIST_HEAD(tmp);
    LIST_HEAD(new_head);

    for (int i = 0; i < split_time; i = i + 1) {
        int j = 0;
        list_for_each (tail, head) {
            if (j >= k) {
                break;
            }
            j = j + 1;
        }
        list_cut_position(&tmp, head, tail->prev);
        q_reverse(&tmp);
        list_splice_tail_init(&tmp, &new_head);
    }
    list_splice_init(&new_head, head);
}
