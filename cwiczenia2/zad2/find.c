#include <stdlib.h>
#include <stdlib.h>
#include <dirent.h>
#include <zconf.h>
#include <ftw.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>

void findRecursively(char* path) {

}

/**
 * Parses to timestamp from format
 * <hh:mm:ss DD:MM:YYYY>
 **/
time_t get_timestamp_from(char *date)
{
    int h, m, s, D, M, Y;
    sscanf(date, "%d:%d:%d %d-%d-%d", &h, &m, &s, &D, &M, &Y);

    time_t time_now = time(NULL);
    struct tm *local_time = localtime(&time_now);
    struct tm mk_time_input = {
        .tm_hour = h, .tm_min = m, .tm_sec = s, .tm_mday = D, .tm_mon = M - 1, .tm_year = Y - 1900, .tm_zone = local_time->tm_zone, .tm_gmtoff = local_time->tm_gmtoff, .tm_isdst = local_time->tm_isdst};

    return mktime(&mk_time_input) + timezone;
}

void print_file_type(struct stat *sb)
{
    printf("File type:                ");
    switch (sb->st_mode & S_IFMT)
    {
    case S_IFREG:
        printf("file\n");
        break;
    case S_IFBLK:
        printf("block dev\n");
        break;
    case S_IFCHR:
        printf("char device\n");
        break;
    case S_IFDIR:
        printf("dir\n");
        break;
    case S_IFIFO:
        printf("fifo\n");
        break;
    case S_IFLNK:
        printf("slink\n");
        break;
    case S_IFSOCK:
        printf("sock\n");
        break;
    default:
        printf("unknown?\n");
        break;
    }
}

bool modification_date_matches(time_t* file_timestamp, char* requested_date, char* operator) {
    // TODO i think parsing arguments should be handled as close to main() as possible
    // does C have way of holding below state?
    bool findModifiedBefore = strcmp(operator, "<") == 0;
    bool findModifiedAt = strcmp(operator, "=") == 0;
    bool findModifiedAfter = strcmp(operator, ">") == 0;

    time_t requested_date_timestamp = get_timestamp_from(requested_date);

    // puts("file: ");
    // puts(ctime(file_timestamp));
    // puts("requested_date_timestamp: ");
    // puts(ctime(&requested_date_timestamp));
    // printf("%d", findModifiedBefore);

    if(findModifiedAfter) {
        return *file_timestamp > requested_date_timestamp;
    } else if(findModifiedAt) {
        return *file_timestamp == requested_date_timestamp;
    } else if (findModifiedBefore) {
        return *file_timestamp < requested_date_timestamp;
    }

    fprintf(stderr, "wrong comparator passed, use '<' | '>' | '='");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    tzset();

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <pathname> '<' | '>' | '=' <hh:mm:ss DD:MM:YYYY>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR *dir;
    struct dirent *dent;
    char buffer[50];
    strcpy(buffer, argv[1])
    ;
    dir = opendir(buffer);
    
    struct stat *sb = malloc(sizeof stat);
    
    if (dir != NULL)
    {
        while ((dent = readdir(dir)) != NULL)
        {

            // name
            char *current_working_directory = malloc(sizeof(char) * 256);

            chdir(argv[1]);
            getcwd(current_working_directory, 256);


            if (lstat(dent->d_name, sb) == -1)
            {
                perror("lstat");
                exit(EXIT_FAILURE);
            }

            if(!modification_date_matches(&sb->st_mtime, argv[3], argv[2])) {
                continue; // ?
            };


            puts(dent->d_name);
            puts(current_working_directory);

            print_file_type(sb);

            printf("File size:                %jd bytes\n",
                   (intmax_t)sb->st_size);


            printf("Last file access:         %s", ctime(&sb->st_atime));
            printf("Last file modification:   %s\n\n", ctime(&sb->st_mtime));
        }
    }
    close(dir);

    return 0;
}