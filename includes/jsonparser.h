#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_BEGIN_ARRAY(chr) (chr == '[')
#define IS_BEGIN_OBJECT(chr) (chr == '{')
#define IS_END_ARRAY(chr) (chr == ']')
#define IS_END_OBJECT(chr) (chr == '}')
#define IS_SPACE(chr) (chr == ' ')
#define IS_NAME_SEPARATOR(chr) (chr == ':')
#define IS_VALUE_SEPARATOR(chr) (chr == ',')
#define IS_QUOTATION_MARK(chr) (chr == '"')
#define IS_END_OBJECT_AND_VALUE_SEPARATOR_NOT_PRESENT(str) (str[0] == '}' && str[1] != ',')

typedef int json_number_t;
typedef char *json_string_t;
typedef struct json_member json_member_t;
typedef struct json_object json_object_t;
typedef struct json_array json_array_t;

typedef struct json_member
{
    int is_array;
    json_string_t string;
    void *value;
    void **array;
    json_object_t *object;
    struct json_member *next;
} json_member_t;

struct json_object
{
    json_member_t *member;
};

struct json_array
{
    int index;
    char *object;
    json_array_t *next;
};


// jsonparser.c

////////////////////////////////////////////
//       Object function                  //
////////////////////////////////////////////

int json_parse_member(char **buffer, json_member_t **member);
int json_parse_object(char **buffer, size_t buffer_size, json_object_t **object);
json_member_t *json_get_member(json_object_t *object, char *key);

////////////////////////////////////////////
//       Array function                  //
///////////////////////////////////////////
int json_parse_array(char **buffer, json_array_t **array);

// jsonparser_memory.c

///////////////////////////////////////////////
//       Memory function                    //
//////////////////////////////////////////////

int allocate_json_object_memory(json_object_t **object);
int allocate_json_object_member_memory(json_member_t **member);
int allocate_json_array_memory(json_array_t **array);
void push_member(json_member_t **first, json_member_t *new);
void push_array(json_array_t **first, json_array_t *new);
void pop_member(json_member_t **first);
void pop_array(json_array_t **first);
void free_array(json_array_t **array);
void free_member_array(json_member_t *member);
void free_member(json_member_t **first);
void free_object(json_object_t *object);