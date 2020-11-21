#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

// watch -f *.c -r 'make compile'

void generate(const char* file_name, const int record_count, const int record_size);

void sort_sys(const char *file_name, const int record_count, const int record_size);
void sort_lib(const char *file_name, const int record_count, const int record_size);

void copy_sys(const char *source, const char *destination, int record_count, int buffer_size);
void copy_lib(const char *source, const char *destination, int record_count, int buffer_size);

int main(int argc, char** argv) {

    bool should_generate = strcmp("generate", argv[1]) == 0;
    bool should_sort = strcmp("sort", argv[1]) == 0;
    bool should_copy = strcmp("copy", argv[1]) == 0;

    if(should_generate) {

        char** file_name = argv[2];
        int record_count;
        int record_size;
        sscanf(argv[3], "%i", &record_count);
        sscanf(argv[4], "%i", &record_size);

        // puts(file_name);
        // printf("%d", record_count);

        generate(file_name, record_count, record_size);
    } else if(should_sort) {
        char** file_name = argv[2];
        int record_count;
        int record_size;
        char** type = argv[5];

        sscanf(argv[3], "%i", &record_count);
        sscanf(argv[4], "%i", &record_size);

        if(strcmp("sys", type) == 0) {
            sort_sys(file_name, record_count, record_size);
        } else {
            sort_lib(file_name, record_count, record_size);
        };
        // puts(type);

    } else if(should_copy) {
        char** src_path = argv[2];
        char** dest_path = argv[3];
        int record_count;
        int buffer_size;
        char** type = argv[5];

        sscanf(argv[4], "%i", &record_count);
        sscanf(argv[5], "%i", &buffer_size);

        // puts(src_path);
        // puts(dest_path);
        // printf("%d", record_count);
        // printf("%d", buffer_size);

        if(strcmp("sys", type) == 0) {
            copy_sys(src_path, dest_path, record_count, buffer_size);
        } else {
            copy_lib(src_path, dest_path, record_count, buffer_size);
        };
    }

    return 0;
}


void copy_sys(const char *source, const char *destination, int record_count, int buffer_size) {
    int src_file = open(source, O_RDONLY);
    int dest_file = open(destination, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    
    unsigned char buffer[buffer_size + 1];
    for (int i = 0; i < record_count; i++) {
        read(src_file, &buffer, sizeof(buffer));
        write(dest_file, &buffer, sizeof(buffer));
    }
}

void copy_lib(const char *source, const char *destination, int record_count, int buffer_size) {
    FILE *src_file = fopen(source, "r");
    FILE *dest_file = fopen(destination, "w");

    unsigned char buffer[buffer_size + 1];
    for (int i = 0; i < record_count; i++) {
        fread(&buffer, sizeof(unsigned char), (size_t) buffer_size + 1, src_file);
        fwrite(&buffer, sizeof(unsigned char), (size_t) buffer_size + 1, dest_file);
    }
}


void sort_sys(const char *file_name, const int record_count, const int record_size) {
    int file = open(file_name, O_RDWR);

    for (int i = 0; i < record_count - 1; i++) {
        unsigned char key[record_size + 1];
        unsigned char ref_key[record_size + 1];

        int min = i;
        
        for (int j = i + 1; j < record_count + 1; j++) {
            // printf("j: %d\n", j);
            lseek(file, j * sizeof(key), SEEK_SET);
            read(file, &key, sizeof(key));

            lseek(file, min * sizeof(ref_key), SEEK_SET);
            read(file, &ref_key, sizeof(ref_key));

            if (key[0] < ref_key[0]) {
                min = j;
            }
        }
        if (min != i) {

            lseek(file, i * sizeof(key), SEEK_SET);
            read(file, &key, sizeof(key));

            lseek(file, min * sizeof(key), SEEK_SET);
            read(file, &ref_key, sizeof(ref_key));

            // printf("%s ", key);
            // printf("%s \n", ref_key);
            // printf("swap \n");

            lseek(file, i * sizeof(key), SEEK_SET);
            write(file, &ref_key, sizeof(ref_key));
            lseek(file, min * sizeof(key), SEEK_SET);
            write(file, &key, sizeof(key));
        }
    }
}

void sort_lib(const char *file_name, const int record_count, const int record_size) {
    FILE *file = fopen(file_name, "r+");

    for (int i = 0; i < record_count - 1; i++) {
        unsigned char key[record_size + 1];
        unsigned char ref_key[record_size + 1];

        int min = i;
        
        for (int j = i + 1; j < record_count + 1; j++) {
            // printf("j: %d\n", j);
            fseek(file, j * sizeof(key), SEEK_SET);
            fread(key, sizeof(unsigned char), (size_t) record_size, file);

            fseek(file, min * sizeof(ref_key), SEEK_SET);
            fread(ref_key, sizeof(unsigned char), (size_t) record_size, file);

            if (key[0] < ref_key[0]) {
                min = j;
            }
        }
        if (min != i) {
            fseek(file, i * sizeof(key), SEEK_SET);
            fread(key, sizeof(unsigned char), (size_t) record_size, file);

            fseek(file, min * sizeof(key), SEEK_SET);
            fread(ref_key, sizeof(unsigned char), (size_t) record_size, file);

            // printf("%s ", key);
            // printf("%s \n", ref_key);
            // printf("swap \n");

            fseek(file, i * sizeof(key), SEEK_SET);
            fwrite(&ref_key, sizeof(unsigned char), (size_t) record_size, file);
            fseek(file, min * sizeof(key), SEEK_SET);
            fwrite(&key, sizeof(unsigned char), (size_t) record_size, file);
        }
    }
}

void generate(const char* file_name, const int record_count, const int record_size) {
    FILE* file = fopen(file_name, "w+");

    FILE* random_file = fopen("/dev/urandom", "r");
    char* record = malloc(record_size * sizeof(char) + 1);

    // generate record_count rows
    for (int i = 0; i < record_count; ++i) {

        int read_result = fread(
            record,
            sizeof(char),
            (size_t) record_size + 1,
            random_file
        );

        if(read_result != record_size + 1) {
            printf("error reading /dev/urandom");
        };

        // convert
        const int alphanumeric_count = 25;
        for (int j = 0; j < record_size; j++) {
            record[j] = (char) (abs(record[j]) % alphanumeric_count + 'a');
        }
        record[record_size] = '\n';

        int write_result = fwrite(
            record, 
            sizeof(char), 
            (size_t)record_size + 1,
            file
        );

        if (write_result != record_size + 1) {
            printf("error writing to file\n");
        }
    }
    fclose(file);
    fclose(random_file);
    free(record);
}