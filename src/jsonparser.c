#include "jsonparser.h"

////////////////////////////////////////////
//       Object function                  //
////////////////////////////////////////////


int json_parse_member(char **buffer, json_member_t **member)
{
    char *tmp = ++buffer[0];
    long to_dec = 0;
    int i = 1;

    while (**buffer && !IS_QUOTATION_MARK(**buffer))
        buffer[0]++;

    **buffer = 0;
    (*member)->string = tmp;

    if (IS_NAME_SEPARATOR(**buffer))
        tmp = (buffer[0] + 2);
    
    buffer[0] += 2;


    while (**buffer && **buffer <= 32)
        buffer[0]++;
    
    if (IS_BEGIN_OBJECT(**buffer)) {
        json_object_t *new_ptr = (json_object_t *)malloc(sizeof(json_object_t));

        json_parse_object(buffer,strlen(*buffer)-1,&new_ptr);

        (*member)->object = new_ptr;

        while (**buffer && !IS_QUOTATION_MARK(**buffer))
            buffer[0]++;
        
        return (0);
    }
    
    if (IS_QUOTATION_MARK(**buffer)) {
        tmp = ++buffer[0];

        while (**buffer && !IS_QUOTATION_MARK(**buffer))
            buffer[0]++;
        
        **buffer = 0;
        (*member)->value = tmp;
        buffer[0]++;

    }
    
    if (**buffer >= '0' && **buffer <= '9') {
        
        tmp = buffer[0];

        while (**buffer && **buffer >= '0' && **buffer <= '9') {
            to_dec *= 10;
            to_dec += **buffer & 0xF;
            buffer[0]++;
        }

        (*member)->value = (long *)to_dec;
    }

    if (strncmp(buffer[0],"true",4) == 0 || strncmp(buffer[0],"false",5) == 0) {
        tmp = buffer[0];

        while (**buffer && **buffer > ' ' && !IS_VALUE_SEPARATOR(**buffer))
            buffer[0]++;
        
        **buffer = 0;

        (*member)->value = tmp;
        tmp = ++buffer[0];
    }

    if (IS_BEGIN_ARRAY(**buffer)) {
        (*member)->is_array = 1;
        tmp = ++buffer[0];
        char *tmp_ = buffer[0];

        while (!IS_END_ARRAY(*tmp_)) {
            
            if (IS_VALUE_SEPARATOR(*tmp_))
                i++;
            
            tmp_++;
        }

        (*member)->array = (void **)malloc(sizeof(void *) * i);

        for (int x = 0; x < i;) {
            
            while (**buffer && !IS_VALUE_SEPARATOR(**buffer) && !IS_END_ARRAY(**buffer))
                buffer[0]++;

            **buffer = 0;
            
            (*member)->array[x++] = tmp;

            while (**buffer <= ' ')
                buffer[0]++;

            tmp = buffer[0];
        }
    }

    return (0);
}

int json_parse_object(char **buffer, size_t buffer_size, json_object_t **object)
{
    json_member_t *new = NULL;

    if (!IS_BEGIN_OBJECT(**buffer))
        return (1);

    if (!IS_END_OBJECT(buffer[0][buffer_size]))
        return (1);
    
    while (**buffer && !IS_QUOTATION_MARK(**buffer) && !IS_END_OBJECT(**buffer))
        buffer[0]++;
    
    while (**buffer && !IS_END_OBJECT(**buffer)) {

        if (allocate_json_object_member_memory(&new))
            return (1);

        json_parse_member(buffer,&new);

        push_member(&(*object)->member,new);

        while (**buffer && !IS_VALUE_SEPARATOR(**buffer) && !IS_END_OBJECT(**buffer) && !IS_QUOTATION_MARK(**buffer))
            buffer[0]++;
        
        if (IS_VALUE_SEPARATOR(**buffer)) {
            while (**buffer && !IS_QUOTATION_MARK(**buffer))
                buffer[0]++;
        }
    }

    return (0);
}

json_member_t *json_get_member(json_object_t *object, char *key)
{
    json_member_t *actual = object->member;

    while (actual && strcmp(actual->string,key))
        actual = actual->next;
    
    return (actual);
}

////////////////////////////////////////////
//       Array function                  //
///////////////////////////////////////////

int json_parse_array(char **buffer, json_array_t **array)
{

    if (!IS_BEGIN_ARRAY(**buffer))
        return (1);
    
    char *tmp = NULL;
    int i = 0;
    json_array_t *array_ptr = NULL;

    while (**buffer && !IS_END_ARRAY(**buffer)) {

        while (**buffer && !IS_BEGIN_OBJECT(**buffer))
            buffer[0]++;
        
        tmp = buffer[0];

        while (**buffer && !IS_END_OBJECT(**buffer) && !IS_BEGIN_OBJECT(**buffer))
            buffer[0]++;
        
        if (IS_BEGIN_OBJECT(**buffer)) {
            while (**buffer && !IS_END_OBJECT(**buffer))
                buffer[0]++;
        }

        if (IS_END_OBJECT(**buffer)) {
            buffer[0]++;
            
            if (IS_VALUE_SEPARATOR(**buffer)) {
                while (**buffer && !IS_END_OBJECT(**buffer))
                    buffer[0]++;
            }
            buffer[0]++;
            **buffer = 0;
        } else {
            while (**buffer && !IS_END_OBJECT(**buffer))
                buffer[0]++;
        }         

        if (allocate_json_array_memory(&array_ptr))
            return (1);
        
        array_ptr->index = i++;
        array_ptr->object = tmp;

        push_array(array, array_ptr);

        tmp = ++buffer[0];
    }

    return (0);
}