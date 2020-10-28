#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void print_usage(char pwd[]);

void generate(const char *path, int rowc, int row_size);

void sort_sys(const char *path, int rowc, int row_size);

void sort_lib(const char *path, int rowc, int row_size);

void copy_sys(const char *src_path, const char *dest_path, int rowc, int buffer_size);

void copy_lib(const char *src_path, const char *dest_path, int rowc, int buffer_size);

int main(int argc, char *argv[]) {
    int rowc;
    int row_size;
    int buffer_size;

    int row_sscanf = argc > 4 && sscanf(argv[3], "%i", &rowc) && sscanf(argv[4], "%i", &row_size);
    int row_buffer_sscanf = argc == 7 && sscanf(argv[4], "%i", &rowc) && sscanf(argv[5], "%i", &buffer_size);

    int is_generate = row_sscanf && argc == 5 && strcmp("generate", argv[1]) == 0;
    int is_sort_sys = row_sscanf && argc == 6 && strcmp("sort", argv[1]) == 0 && strcmp("sys", argv[5]) == 0;
    int is_sort_lib = row_sscanf && argc == 6 && strcmp("sort", argv[1]) == 0 && strcmp("lib", argv[5]) == 0;
    int is_copy_sys = row_buffer_sscanf && strcmp("copy", argv[1]) == 0 && strcmp("sys", argv[6]) == 0;
    int is_copy_lib = row_buffer_sscanf && strcmp("copy", argv[1]) == 0 && strcmp("lib", argv[6]) == 0;

    if (is_generate) {
        generate(argv[2], rowc, row_size);
    } else if (is_sort_sys) {
        sort_sys(argv[2], rowc, row_size);
    } else if (is_sort_lib) {
        sort_lib(argv[2], rowc, row_size);
    } else if (is_copy_sys) {
        copy_sys(argv[2], argv[3], rowc, buffer_size);
    } else if (is_copy_lib) {
        copy_lib(argv[2], argv[3], rowc, buffer_size);
    } else {
        print_usage(argv[0]);
        return -1;
    }

    return 0;
}

void print_usage(char pwd[]) {
    printf("Usage:\n");
    printf("%s generate file rowc row_size\n", pwd);
    printf("%s sort file rowc row_size [sys | lib]\n", pwd);
    printf("%s copy source_file dest_file rowc buffer_size [sys | lib]\n", pwd);
}

void generate(const char *path, int rowc, int row_size) {
    int dest = open(path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    int random = open("random", O_RDONLY);
    int tmp = 0;
    for (int i = 0; i < rowc; i++) {
        for (int j = 0; j < row_size; j++) {
            while (tmp > 57 || tmp < 48) {
                read(random, &tmp, 1);
            }
            write(dest, &tmp, 1);
            tmp = 0;
        }
        write(dest, "\n", 1);
    }
}

void sort_sys(const char *path, int rowc, int row_size) {
    int file = open(path, O_RDWR);
    for (int j = 1; j < rowc; j++) {
        unsigned char key[row_size + 1];
        lseek(file, j * sizeof(key), SEEK_SET);
        read(file, &key, sizeof(key));

        int i = j - 1;
        unsigned char i_row[row_size + 1];
        lseek(file, i * sizeof(i_row), SEEK_SET);
        read(file, &i_row, sizeof(i_row));

        while (i > -1 && i_row[0] > key[0]) {
            lseek(file, (i + 1) * sizeof(i_row), SEEK_SET);
            write(file, &i_row, sizeof(i_row));
            i--;
            lseek(file, i * sizeof(i_row), SEEK_SET);
            read(file, &i_row, sizeof(i_row));
        }

        lseek(file, (i + 1) * sizeof(key), SEEK_SET);
        write(file, &key, sizeof(key));
    }
}

void sort_lib(const char *path, int rowc, int row_size) {
    FILE *file = fopen(path, "r+");
    for (int j = 1; j < rowc; j++) {
        unsigned char key[row_size + 1];
        fseek(file, j * sizeof(key), SEEK_SET);
        fread(key, sizeof(unsigned char), (size_t) row_size, file);

        int i = j - 1;
        unsigned char i_row[row_size + 1];
        fseek(file, i * sizeof(i_row), SEEK_SET);
        fread(&i_row, sizeof(unsigned char), (size_t) row_size, file);

        while (i > -1 && i_row[0] > key[0]) {
            fseek(file, (i + 1) * sizeof(i_row), SEEK_SET);
            fwrite(&i_row, sizeof(unsigned char), (size_t) row_size, file);
            i--;
            fseek(file, i * sizeof(i_row), SEEK_SET);
            fread(&i_row, sizeof(unsigned char), (size_t) row_size, file);
        }

        fseek(file, (i + 1) * sizeof(key), SEEK_SET);
        fwrite(&key, sizeof(unsigned char), (size_t) row_size, file);
    }
}

void copy_sys(const char *src_path, const char *dest_path, int rowc, int buffer_size) {
    int src = open(src_path, O_RDONLY);
    int dest = open(dest_path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    for (int i = 0; i < rowc; i++) {
        unsigned char buffer[buffer_size + 1];
        read(src, &buffer, sizeof(buffer));
        write(dest, &buffer, sizeof(buffer));
    }
}

void copy_lib(const char *src_path, const char *dest_path, int rowc, int buffer_size) {
    FILE *src = fopen(src_path, "r");
    FILE *dest = fopen(dest_path, "w");
    for (int i = 0; i < rowc; i++) {
        unsigned char buffer[buffer_size + 1];
        fread(&buffer, sizeof(unsigned char), (size_t) buffer_size + 1, src);
        fwrite(&buffer, sizeof(unsigned char), (size_t) buffer_size + 1, dest);
    }
}
