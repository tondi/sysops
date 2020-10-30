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

void generate(char* file_name, int count, int length);

// void print_usage(char pwd[]);

// void generate(const char *path, int rowc, int row_size);

// void sort_sys(const char *path, int rowc, int row_size);

// void sort_lib(const char *path, int rowc, int row_size);

// void copy_sys(const char *src_path, const char *dest_path, int rowc, int buffer_size);

// void copy_lib(const char *src_path, const char *dest_path, int rowc, int buffer_size);


print_array(int *array, int length)
{
    for (int i = 0; i < length; i++) { 
        printf("%c\n ", array[i]);
     }
}

int main(int argc, char** argv) {

    bool should_generate = strcmp("generate", argv[1]) == 0;

    if(should_generate) {

        char** file_name = argv[2];
        int record_count;
        int record_size;
        sscanf(argv[3], "%i", &record_count);
        sscanf(argv[4], "%i", &record_size);

        // puts(file_name);
        // printf("%d", record_count);

        generate(file_name, record_count, record_size);
    }

    // bool should_generate = strcmp("generate", argv[1]) == 0;

    // printf("%d", should_generate);

    // if(should_generate) {
    //     generate("dane.txt", 20, 4);
    // }

    return 0;
}

// record_count = ilosc rekordow
void generate(char* file_name, int record_count, int record_size) {
    FILE* file = fopen(file_name, "w+");

    FILE* random_file = fopen("/dev/urandom", "r");
    char* record = malloc(record_size * sizeof(char) + 1);

    // tyle rekordow do wygenerowania
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