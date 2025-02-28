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
