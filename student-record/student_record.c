#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <getopt.h>
#include <string.h>
#include "defs.h"

char* clone_string(char* original)
{
	char* copy;
	copy=malloc(strlen(original)+1);
	strcpy(copy,original);
	return copy;
}

bool streq(char* str1, char* str2, size_t size)
{
    bool ret = true;
    for (size_t i = 0; i < size; i++)
    {
       if (str1[i] != str2[i])
       {
            ret = false;
            break;
       }
    }
    return ret;
}

student_t* parse_records(char* records_[], int nr_records)
{
    student_t* students;

    char* token; 

    students = malloc(sizeof(records_t) * nr_records);
    
    for (int i = 0; i < nr_records; ++i)
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
            token_id++;
        }
        /*Delete the /n of each entry*/
        students[i].last_name[strlen(students[i].last_name) - 1] = 0x00;
    }
    return students;
}

static void free_entries(student_t* entries, int nr_records)
{
	int i=0;
	for (i=0; i<nr_records; i++) {
		free(entries[i].first_name);
		free(entries[i].last_name);
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
    strcpy(student_serialized, student_id_c);
    strcat(student_serialized, ":");
    strcat(student_serialized, student.NIF);
    strcat(student_serialized, ":");
    strcat(student_serialized, student.first_name);
    strcat(student_serialized, ":");
    strcat(student_serialized, student.last_name);
}

void dump_entries(records_t records, FILE* students)
{
    fprintf(students, "%d", records.records_len);
    for (size_t i = 0; i < records.records_len; i++)
    {
        char studentT[MAX_STUDENT_ENTRY_SIZE];
        studentToChar(records.records[i], studentT);
        fprintf(students, "\n%s", studentT);
    }
}

char** loadstr(FILE* students, int* nr_entry)
{
    char line[MAX_STUDENT_ENTRY_SIZE];//lenght
    int line_count = 0;
    char** entries;
    entries = malloc(sizeof(char*));
    /*Check length*/
    int i = 0;
    int b = 0;
    if (students == stdin)
    {
        printf("Please input number of entries to load: ");
        scanf("%d", &b);
        getchar();
    }
    else
    {
        b = 10;
    }
        
    while ((i < b) && fgets(line, MAX_STUDENT_ENTRY_SIZE, students) != NULL){
		line_count++;
        entries[i] = clone_string(line);
        if (students != stdin)
        {
            b++;
        }
        i++;     
        entries = realloc(entries, sizeof(char*) +  sizeof(char*) * i);
	}

    *nr_entry = line_count;
    return entries;
}

records_t read_student_file(FILE* students, options_t options)
{
    records_t ret;
    
    if (students != stdin)
    {
        /*Load the 4 first bytes as the header*/
        char num[10];
        fgets(num, 10, students);
        sscanf(num, "%d", &ret.records_len);
    }
    int nr_read_Entries;
    char** records = loadstr(students, &nr_read_Entries);
    if (students != stdin && nr_read_Entries != ret.records_len)
    {
        fprintf(stderr, "Error: wrong entry size in file.\n");
        exit(0);
    }
    else if(students == stdin)
    {
       ret.records_len = nr_read_Entries;
    }
    
    /*Load the rest of the file*/
    ret.records = parse_records(records, ret.records_len);

    /*Free the records string array*/
    for (size_t i = 0; i < nr_read_Entries; i++)
    {
        free(records[i]);
    }
    free(records);
    
    return ret;
}

void listStudents(FILE* outFile, options_t options)
{
    records_t records;
    records = read_student_file(options.inFile, options);
    for (int i = 0; i < records.records_len; i++)
    {
        fprintf(outFile, "[Entry # %d]\n", i);
        fprintf(outFile, "\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
        records.records[i].student_id, records.records[i].NIF, records.records[i].first_name, records.records[i].last_name);
    }
}

void createStudents(options_t options)
{
    records_t records;
    records = read_student_file(stdin, options);
    dump_entries(records, options.inFile);
    free_entries(records.records, records.records_len);
}

void cpyStudentArray(size_t entries, student_t* src, student_t* dst, int start)
{
    for (size_t i = 0; i < entries; i++)//start position is copied
    {
        dst[i + start].student_id = src[i].student_id;
        strcpy(dst[i + start].NIF, src[i].NIF);
        dst[i + start].first_name = src[i].first_name;
        dst[i + start].last_name = src[i].last_name;
    }
}

student_t* realloc_s(student_t* src, size_t dst_bytes, size_t src_bytes)
{
    student_t* dst = malloc(dst_bytes);
    cpyStudentArray(src_bytes / sizeof(student_t), src, dst, 0);    
    //free_entries(src, src_bytes / sizeof(student_t));
    return dst;
}

void recordMerger(records_t* records,  records_t records_a)
{
    /*check for duplicates*/ 

    for (size_t i = 0; i < records->records_len; i++)
    {
        for (size_t j = 0; j < records_a.records_len; j++)
        {
            if (streq(records->records[i].NIF, records_a.records[i].NIF, 10))
            {
                fprintf(stderr, "Found duplicate student with NIF: %s ", records->records[i].NIF);
                perror(NULL);
                exit(EXIT_FAILURE);
            }
            if (records->records[i].student_id == records_a.records[i].student_id)
            {
                fprintf(stderr, "Found duplicate student with id: %d ", records->records[i].student_id);
                perror(NULL);
                exit(EXIT_FAILURE);
            }
        } 
    }

    records->records = realloc_s(records->records, sizeof(student_t) * (records->records_len + records_a.records_len), sizeof(student_t) * records->records_len);

    cpyStudentArray(records_a.records_len, records_a.records, records->records, records->records_len);

    records->records_len += records_a.records_len;  

    //free_entries(records_a.records, records_a.records_len); 
}

void queryStudent(options_t options, char* ID)
{
    bool found = false;
    /*Load students to memory from existing file*/
    records_t records;
    char ID_c[10];
    records = read_student_file(options.inFile, options);
    for (size_t i = 0; i < records.records_len && !found; i++)
    {
       
        if (options.qMode == q_ID)
        {
            sprintf(ID_c, "%d", records.records[i].student_id);
            if (streq( ID_c, ID, strlen(ID)))
            {
                found = true;
                //Print the user with the specified student_id
                fprintf(stdout, "\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
                records.records[i].student_id, records.records[i].NIF, records.records[i].first_name, records.records[i].last_name);
            }            
        }
        else
        {
            if (streq(records.records[i].NIF, ID, strlen(ID)))
            {
                found = true;
                fprintf(stdout, "\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n",
                records.records[i].student_id, records.records[i].NIF, records.records[i].first_name, records.records[i].last_name);
                //Print the user with the specified NIF
            }  
        }       
    }
    if (!found)
    {
        fprintf(stdout, "User not found\n");
    }
    
    free_entries(records.records, records.records_len);
}

void appendStudents(options_t options)
{
    /*Load students to memory from existing file*/
    records_t records;
    records = read_student_file(options.inFile, options);
    /*Load students to memory from user input*/
    records_t records_a;
    records_a = read_student_file(stdin, options);
    /*Check for duplicates and merge the two student sets*/
    recordMerger(&records, records_a);
    /*Delete contents of file*/
    options.inFile = freopen(NULL, "w", options.inFile);
    /*Write back to file*/
    dump_entries(records, options.inFile);
    /*Free memory*/
    //free_entries(records_a.records, records_a.records_len);
}

void openFile(char* mode, options_t *options)
{
    if ((options->inFile=fopen(options->inFilec, mode))==NULL) 
    {
        fprintf(stderr, "The file %s could not be opened: ", optarg);
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
	while((opt = getopt(argc, argv, "f:hlcaq:")) != -1 && !rec) {
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
            openFile("w", &options);
            createStudents(options);
            fclose(options.inFile);
			break;
		case 'a':
			options.action=APPEND_MODE;
            rec = true;
            openFile("r+", &options);
            appendStudents(options);
            fclose(options.inFile);
			break;
		case 'q':
			options.action=QUERY_MODE;
            bool sopt = false;
            rec = true;
            if ((opt = getopt(argc, argv, "i:n:")) != -1)
            {
                switch (opt)
                {
                case 'i':
                    options.qMode = q_ID;
                    sopt = true;
                    queryStudent(options, optarg);
                    break;
                
                case 'n':
                    options.qMode = q_NIF;
                    sopt = true;
                    queryStudent(options, optarg);
                    break;

                default:
                    if (!sopt)
                    {
                        fprintf(stderr, "Query mode not specified");
                        exit(EXIT_FAILURE);
                    }
                    break;
                }
            }
            
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