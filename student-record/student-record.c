#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <getopt.h>
#include <string.h>
#include "defs.h"

static inline char* clone_string(char* original)
{
	char* copy;
	copy=malloc(strlen(original)+1);
	strcpy(copy,original);
	return copy;
}

student_t* parse_records(char* records_[], int* nr_records)
{
    char* token; 
    student_t* students;
    int entrynum;
    entrynum = sizeof records_ / sizeof records_[0];

    students = malloc(sizeof(records_t) * entrynum);
    
    for (int i = 0; i < entrynum; ++i)
    {
        token_id_t token_id=STUDENT_ID;
        while((token = strsep(&records_[i], ":"))!=NULL) {
            switch (token_id)
            {
                case STUDENT_ID:
                    if (sscanf(token,"%d",&students[i].student_id)!=1) {
                        fprintf(stderr, "Warning: Wrong format for student_id field");
                        students[i].student_id=0;
				    }
                    break;

                case NIF:
                    strcpy(students[i].NIF,token);
                    break;

                case NAME:
                   students[i].first_name = clone_string(token);
                    break;

                case SURNAME:
                    students[i].last_name = clone_string(token);
                    break;
            
                default:
                    break;
            }
            
        }
        token_id++;
    }
    *nr_records = entrynum; 
    return students;
}

static void free_entries(student_t* entries, int nr_records)
{
	int i=0;
	student_t* entry;
	for (i=0; i<nr_records; i++) {
		entry=&entries[i]; /* Point to current entry */
		free(entry->NIF);
		free(entry->first_name);
		free(entry->last_name);
	}
	free(entries);
}

/* char studentT[MAX_STUDENT_ENTRY_SIZE]; */
void studentToChar(student_t student, char* student_serialized)
{
    /* Converting from student_id (int) to char* */
    char student_id_c[4];
    sprintf(student_id_c, "%d", student.student_id);

    /*Append student to a char* */
    strcat(student_serialized, student_id_c);
    strcat(student_serialized, student.NIF);
    strcat(student_serialized, student.first_name);
    strcat(student_serialized, student.first_name);
}

void dump_entries(records_t records, FILE* students)
{
    char studentT[MAX_STUDENT_ENTRY_SIZE];
    for (size_t i = 0; i < records.records_len; i++)
    {
        studentToChar(records.records[i], studentT);
        fwrite(studentT, MAX_STUDENT_ENTRY_SIZE, 1, students);
    }
}

char** loadstr(FILE* students)
{
    char line[MAX_STUDENT_ENTRY_SIZE];//lenght
    int line_count = 0;
    char** entries;
    /*Check length*/
    while (fgets(line, MAX_STUDENT_ENTRY_SIZE, students) != NULL){
		line_count++;
	}
    /*Reset pointer*/
    fseek(students, -(line_count+1) * MAX_STUDENT_ENTRY_SIZE,SEEK_CUR);

    /*Reserve memory*/
    entries = malloc(MAX_STUDENT_ENTRY_SIZE * (line_count + 1));

    memset(entries,0,MAX_STUDENT_ENTRY_SIZE * (line_count + 1));

    /*Load to entries array*/

    int bytesR = 0;
    for (size_t i = 0; i < line_count; i++)
    {
        bytesR = fgets(line, MAX_STUDENT_ENTRY_SIZE, students);
        strcpy(entries[i], line);
    }
    return entries;
}

records_t read_student_file(FILE* students)
{
    records_t ret;
    int length;

    /*Load the 4 first bytes as the header*/
    char num[10];
    fgets(num, 10, students);
    sscanf(num, "%d", &ret.records_len);

    /*Load the rest of the file*/
    ret.records = parse_records(loadstr(students), &length);

    if (length != ret.records_len)
    {
        fprintf(stderr, "Wrong entry size");
    }
    
    return ret;
}

void listStudents(FILE* outfile, options_t options)
{
    records_t records;
    records = read_student_file(options.inFile);
    for (int i = 0; i < records.records_len; i++)
    {
        printf("[Entry # %d]\n", i);
        printf("\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
        records.records[i].student_id, records.records[i].NIF, records.records[i].first_name, records.records[i].last_name);
    }
}

void openFile(char* mode, options_t *options)
{
    if ((options->inFile=fopen(options->inFilec, "r"))==NULL) 
    {
        fprintf(stderr, "The output file %s could not be opened: ", optarg);
        perror(NULL);
        exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[])
{
    int opt;
    options_t options;
    records_t records;
    bool rec = false;

    /*Check #arguments*/
    if (argc < 4 )
    {
        fprintf(stderr,"Usage: %s -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ] ] [ list of records ]\n",argv[0]);
		exit(0);
    }

    /* Parse command-line options */
	while((opt = getopt(argc, argv, "f:hlc:a:q:")) != -1 && !rec) {
		switch(opt) {
        case 'f':
            strcpy(options.inFilec, optarg);
			break;
		case 'h':
			fprintf(stderr,"Usage: %s -f file [ -h | -l | -c | -a | -q [ -i|-n ID] ] ] [ list of records ]\n",argv[0]);
			exit(0);
		case 'l':
			options.action=LIST_MODE;
            rec = true; 
            openFile("r", &options);
            listStudents(stdout, options);
			break;
		case 'c':
			options.action=CREATE_MODE;
            rec = true;
            //records.records = parse_records(records_, records.records_len);

            free_entries(records.records, records.records_len);
			break;
		case 'a':
			options.action=APPEND_MODE;
            rec = true;
            //records.records = parse_records(records_, records.records_len);

            free_entries(records.records, records.records_len);
			break;
		case 'q':
			options.action=QUERY_MODE;
            //Check type of query mode
            rec = true;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}
    /*options error checking*/
    if (options.inFile == NULL)
    {
        fprintf(stderr, "Database not specified");
		perror(NULL);
        exit(EXIT_FAILURE);
    }
    return 0;
}