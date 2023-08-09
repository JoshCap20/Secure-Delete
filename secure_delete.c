#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define FILE_TYPE 1
#define DIRECTORY_TYPE 2

int delete_file(char *filepath);
int delete_directory(char *filepath);
int get_file_type(char *filepath);
void take_action(char *filepath);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "This command only accepts one arguments.\n");
        exit(1);
    }

    take_action(argv[1]);

    exit(0);
}

void take_action(char *filepath)
{
    switch (get_file_type(filepath))
    {
    case FILE_TYPE:
        delete_file(filepath);
        break;
    case DIRECTORY_TYPE:
        delete_directory(filepath);
        break;
    default:
        fprintf(stderr, "Invalid file type: %s\n", filepath);
        exit(1);
    }
}

// Returns 0 on success
int overwrite_file(char *filepath)
{
    FILE *file;
    long file_size;

    // Check if file exists
    if ((file = fopen(filepath, "rb+")) == NULL)
    {
        fprintf(stderr, "File does not exist: %s\n", filepath);
        return 1;
    }

    // Get size of file
    fseek(file, 0, SEEK_END);

    if ((file_size = ftell(file)) == -1L)
    {
        fprintf(stderr, "Error determining size of %s\n", filepath);
        fclose(file);
        return 1;
    }
    else if (!file_size)
    {
        fprintf(stderr, "Invalid file size.\n");
        fclose(file);
        return 1;
    }
    fseek(file, 0, SEEK_SET);

    // Replace file contents
    char *overwrite_value = malloc(file_size);
    if (!overwrite_value)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 1;
    }
    memset(overwrite_value, 0, file_size);
    size_t overwrite_size = fwrite(overwrite_value, file_size, 1, file);

    // Clear resources
    free(overwrite_value);
    fclose(file);

    // Verify file update
    if (overwrite_size != 1)
    {
        fprintf(stderr, "Error writing to file: %s\n", filepath);
        return 1;
    }
    return 0;
}

int delete_file(char *filepath)
{
    if (overwrite_file(filepath) == 0)
    {
        // Successful overwrite
        remove(filepath);
        fprintf(stdout, "File deleted successfully: %s\n", filepath);
        return 0;
    }

    fprintf(stderr, "Error deleting file: %s\n", filepath);
    return 1;
}

int delete_directory(char *filepath)
{
    DIR *directory;
    struct dirent *directory_obj;
    char fullpath[2048];

    // Check if directory exists
    if ((directory = opendir(filepath)) == NULL)
    {
        fprintf(stderr, "Invalid directory: %s\n", filepath);
        return 1;
    }

    while ((directory_obj = readdir(directory)) != NULL)
    {
        if (strcmp(directory_obj->d_name, ".") == 0 || strcmp(directory_obj->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(fullpath, sizeof(fullpath), "%s/%s", filepath, directory_obj->d_name);
        take_action(fullpath);
    }

    closedir(directory);

    // Remove directory now
    if (rmdir(filepath) != 0)
    {
        fprintf(stderr, "Error deleting directory: %s\n", filepath);
        return 1;
    }

    fprintf(stdout, "Directory deleted successfully: %s\n", filepath);
    return 0;
}

int get_file_type(char *filepath)
{
    struct stat file_info;
    if (stat(filepath, &file_info) == 0)
    {
        if (S_ISREG(file_info.st_mode))
        {
            return FILE_TYPE;
        }
        else if (S_ISDIR(file_info.st_mode))
        {
            return DIRECTORY_TYPE;
        }
    }
    return -1;
}