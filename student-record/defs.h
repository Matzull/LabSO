#ifndef DEFS_H
#define DEFS_H

#define MAX_CHARS_NIF 9
#define MAX_STUDENT_ENTRY_SIZE 128
#define MAX_INFILE_LEN 256

typedef struct {
    int student_id;
    char NIF[MAX_CHARS_NIF+1];
    char* first_name;
    char* last_name;
} student_t;

typedef struct {
    int records_len;
    student_t* records;
} records_t;

/**
 * actions
 */
typedef enum {
	LIST_MODE,
	CREATE_MODE,
	APPEND_MODE,
    QUERY_MODE,
} actions_t;

typedef enum {
    STUDENT_ID,
    NIF,
    NAME,
    SURNAME
} token_id_t;


/**
 * Structure to hold the "variables" associated with
 * command-line options
 */
typedef struct {
	FILE* inFile;
    char inFilec[MAX_INFILE_LEN];
	actions_t action;
}options_t;

#endif