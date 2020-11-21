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


// can you ever forgive me?
char *operator;
char *date;

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

int child_status;

int nftw_callback(const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    if (!S_ISREG(sb->st_mode) || S_ISLNK(sb->st_mode)) {
        return 0;
    }
    
    pid_t pid = 0;
    int status;

    if (( pid = fork()) < 0) {
        return -1;
    }

    pid = wait(&status);
    if (pid == -1) perror("Failed to wait for child");

    if(!modification_date_matches(&sb->st_mtime, date, operator)) {
        return 0;
    };

    char* resolved_path = malloc(sizeof(char) * 256);
    realpath(path, resolved_path);
    // puts(path);
    // puts(dent->d_name);
    puts(resolved_path);
    // realpath(path, resolved_path)
    // puts();

    print_file_type(sb);
    printf("File size:                %jd bytes\n",
        (intmax_t)sb->st_size);
    printf("Last file access:         %s", ctime(&sb->st_atime));
    printf("Last file modification:   %s\n\n", ctime(&sb->st_mtime));
            

    wait(&status);

    return 0;
}

int main(int argc, char *argv[])
{
    tzset();

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <pathname> '<' | '>' | '=' <hh:mm:ss DD:MM:YYYY>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    operator = argv[2];
    date = argv[3];

    nftw(argv[1], nftw_callback, 256, FTW_PHYS);

    return 0;
}