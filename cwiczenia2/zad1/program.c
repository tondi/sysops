#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

// oblsuga cli
// generowanie
// czas

// watch -f *.c -r 'make compile'

void generate(const char* file_name, const int record_count, const int record_size);

// void print_usage(char pwd[]);

void sort_sys(const char *file_name, const int record_count, const int record_size);
void sort_lib(const char *file_name, const int record_count, const int record_size);

void copy_sys(const char *src_path, const char *dest_path, int record_count, int buffer_size);
void copy_lib(const char *src_path, const char *dest_path, int record_count, int buffer_size);


print_array(int *array, int length)
{
    for (int i = 0; i < length; i++) { 
        printf("%c\n ", array[i]);
     }
}

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
        puts(type);

    } else if(should_copy) {
        char** src_path = argv[2];
        char** dest_path = argv[2];
        int record_count;
        int buffer_size;

        sscanf(argv[3], "%i", &record_count);
        sscanf(argv[4], "%i", &buffer_size);
    }

    // bool should_generate = strcmp("generate", argv[1]) == 0;

    // printf("%d", should_generate);

    // if(should_generate) {
    //     generate("dane.txt", 20, 4);
    // }

    return 0;
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
            // array.swap(i, min);
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
            // array.swap(i, min);
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