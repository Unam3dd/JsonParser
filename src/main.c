#include <stdio.h>
#include "jsonparser.h"

int main(void)
{
    char buffer[] = "[ {\n\"id\":\"hello\", \"age\": 12, \"ne\": [1, 2, 3],\n\"lol\": {\n\"p\": 12 },\n\"newtest\": 17}, {\n\"test\": 9 } ]";
    char *ptr = buffer;
    //json_object_t *object = NULL;
    json_array_t *array = NULL;

    if (allocate_json_array_memory(&array))
        fprintf(stderr,"[-] Error allocating memory !\n");
    
    json_parse_array(&ptr,&array);

    printf("%s\n", array->object);

    
    /*if (allocate_json_object_memory(&object))
        fprintf(stderr,"[-] Error allocate object memory !\n");
    
    if (object == NULL)
        fprintf(stderr,"[-] Error allocate memory !\n");

    if (json_parse_object(&ptr,strlen(buffer)-1,&object))
        fprintf(stderr,"[-] Error parse object !\n");
    
    json_member_t *id = json_get_member(object, "age");

    if (id)
        printf("is array : %d\n", id->is_array);
    
    printf("%s\n", object->member->next->string);

    free_object(object);*/
    
    free_array(&array);


    return (0);
}