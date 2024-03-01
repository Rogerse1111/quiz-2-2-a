#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>

void printFileInfo(const char *path, const struct dirent *entry);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: %s directory_name\n", argv[0]);
        return 1;
    }

    DIR *dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("can't open %s\n", argv[1]);
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", argv[1], entry->d_name);
        printFileInfo(fullPath, entry);
    }

    closedir(dir);
    return 0;
}

void printFileInfo(const char *path, const struct dirent *entry) {
    struct stat fileInfo;
    if (stat(path, &fileInfo) == 0) {
        printf("File name: %s\n", entry->d_name);
        printf("inode number: %lu\n", (unsigned long)fileInfo.st_ino);
        printf("number of links: %lu\n", (unsigned long)fileInfo.st_nlink);
        printf("User ID of owner: %u\n", fileInfo.st_uid);
        printf("Group ID of owner: %u\n", fileInfo.st_gid);
        printf("Size in bytes: %ld bytes\n", fileInfo.st_size);
        printf("Last access: %s", ctime(&fileInfo.st_atime));
        printf("Last modification: %s", ctime(&fileInfo.st_mtime));
        printf("Last status change: %s", ctime(&fileInfo.st_ctime));
        printf("Number of disk blocks allocated: %ld\n", (long)fileInfo.st_blocks);

        // Access mode in octal
        printf("Access mode in octal: %o\n", fileInfo.st_mode & 0xFFF); // Mask to display only permission bits

        // Access mode flags
        printf("Access mode flags: ");
        printf((fileInfo.st_mode & S_IRUSR) ? "r" : "-");
        printf((fileInfo.st_mode & S_IWUSR) ? "w" : "-");
        printf((fileInfo.st_mode & S_IXUSR) ? "x" : "-");
        printf((fileInfo.st_mode & S_IRGRP) ? "r" : "-");
        printf((fileInfo.st_mode & S_IWGRP) ? "w" : "-");
        printf((fileInfo.st_mode & S_IXGRP) ? "x" : "-");
        printf((fileInfo.st_mode & S_IROTH) ? "r" : "-");
        printf((fileInfo.st_mode & S_IWOTH) ? "w" : "-");
        printf((fileInfo.st_mode & S_IXOTH) ? "x" : "-");
        printf("\n\n");
    } else {
        perror("Failed to get file stats");
    }
}
