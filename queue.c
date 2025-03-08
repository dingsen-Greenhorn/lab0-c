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

element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    element_t *first = list_first_entry(head, element_t, list);
    list_del(&first->list);

    if (sp && bufsize) {
        snprintf(sp, bufsize, "%s", first->value);
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
    if (sp && bufsize) {
        snprintf(sp, bufsize, "%s", tail->value);
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
// static inline void swap(struct list_head *node_1, struct list_head *node_2)
// {
//     if (node_1 == node_2)
//         return;
//     struct list_head *node_1_prev = node_1->prev;
//     struct list_head *node_2_prev = node_2->prev;
//     if (node_1->prev != node_2)
//         list_move(node_2, node_1_prev);
//     list_move(node_1, node_2_prev);
// }

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
/* Sort elements of queue in ascending/descending order */

bool cmp(const char *s1, const char *s2)
{
    return strcmp(s1, s2) > 0 ? true : false;
}

void mergeTwoLists(struct list_head *L1, struct list_head *L2, bool descend)
{
    if (!L1 || !L2)
        return;
    struct list_head head;
    INIT_LIST_HEAD(&head);
    while (!list_empty(L1) && !list_empty(L2)) {
        element_t *e1 = list_first_entry(L1, element_t, list);
        element_t *e2 = list_first_entry(L2, element_t, list);
        struct list_head *node = (descend ^ (strcmp(e1->value, e2->value) < 0))
                                     ? L1->next
                                     : L2->next;
        list_move_tail(node, &head);
    }
    list_splice_tail_init(list_empty(L1) ? L2 : L1, &head);
    list_splice(&head, L1);
}
/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    // https://hackmd.io/IKsnn85aRHGMrNcRP7BJ1Q?view#2024q1-Homework1-lab0
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *slow = head;
    const struct list_head *fast = NULL;

    for (fast = head->next; fast != head && fast->next != head;
         fast = fast->next->next)
        slow = slow->next;
    struct list_head left;
    list_cut_position(&left, head, slow);
    q_sort(&left, descend);
    q_sort(head, descend);
    mergeTwoLists(head, &left, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */

int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/

    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return 1;

    struct list_head *tail = head->next, *curr, *tmp;

    list_for_each (curr, head) {
        while (tail != head &&
               strcmp(list_entry(curr, element_t, list)->value,
                      list_entry(tail, element_t, list)->value) < 0) {
            tmp = tail->prev;
            list_del(tail);
            q_release_element(list_entry(tail, element_t, list));
            tail = tmp;
        }
        tail = curr;
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/

    if (!head || list_empty(head))
        return 0;
    if (list_is_singular(head))
        return 1;

    struct list_head *tail = head->next, *curr, *tmp;

    list_for_each (curr, head) {
        while (tail != head &&
               strcmp(list_entry(curr, element_t, list)->value,
                      list_entry(tail, element_t, list)->value) > 0) {
            tmp = tail->prev;
            list_del(tail);
            q_release_element(list_entry(tail, element_t, list));
            tail = tmp;
        }
        tail = curr;
    }

    return q_size(head);
}


/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // Handle empty or null list
    if (!head || list_empty(head))
        return 0;

    // Get the first queue context
    queue_contex_t *first_queue =
        container_of(head->next, queue_contex_t, chain);

    // If there's only one queue, return its size
    if (head->next == head->prev)
        return first_queue->size;

    struct list_head *pos = head->next->next;
    struct list_head *n;

    while (pos != head) {
        n = pos->next;  // Save next pointer before we modify the list

        queue_contex_t *current_queue =
            container_of(pos, queue_contex_t, chain);

        // Move all items from current queue to the first queue
        list_splice_init(current_queue->q, first_queue->q);

        // Reset the size of the processed queue
        current_queue->size = 0;

        pos = n;  // Move to next node
    }

    // Sort the merged queue
    q_sort(first_queue->q, descend);

    // Update the size of the first queue
    first_queue->size = q_size(first_queue->q);

    return first_queue->size;
}

void q_shuffle(struct list_head *head)
{
    if (!head || list_is_singular(head))
        return;
    struct list_head *last = head;
    int size = q_size(head);
    while (size > 0) {
        int index = rand() % size;
        struct list_head *new = last->prev;
        struct list_head *old = new;
        while (index--)
            old = old->prev;
        swap(new, old);
        last = last->prev;
        size--;
    }
    return;
}