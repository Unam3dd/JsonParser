#include "jsonparser.h"

int allocate_json_object_memory(json_object_t ** object)
{
    *object = (json_object_t *)malloc(sizeof(json_object_t));

    return (*object == NULL);
}

int allocate_json_object_member_memory(json_member_t **member)
{
    *member = (json_member_t *)malloc(sizeof(json_member_t));

    return (*member == NULL);
}

int allocate_json_array_memory(json_array_t **array)
{
    *array = (json_array_t *)malloc(sizeof(json_array_t));

    return (*array == NULL);
}

void push_member(json_member_t **first, json_member_t *new)
{
    new->next = *first;
    *first = new;
}

void push_array(json_array_t **first, json_array_t *new)
{
    new->next = *first;
    *first = new;
}

void pop_member(json_member_t **first)
{
    json_member_t *next = (*first)->next;
    free(*first);
    *first = next;
}

void pop_array(json_array_t **first)
{
    json_array_t *next = (*first)->next;
    free(*first);
    *first = next;
}

void free_array(json_array_t **array)
{
    json_array_t *next = NULL;

    while ((*array)) {
        next = (*array)->next;
        free(*array);
        *array = next;
    }
}

void free_member_array(json_member_t *member)
{
    free(member->array);
}

void free_member(json_member_t **first)
{
    json_member_t *next = NULL;

    while (*first) {
        next = (*first)->next;
        
        if ((*first)->is_array)
            free_member_array(*first);
        
        if ((*first)->object != NULL)
            free_object((*first)->object);

        free(*first);
        *first = next;
    }
}

void free_object(json_object_t *object)
{
    free_member(&object->member);
    free(object);
}