#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define FILE_TYPE 1
#define DIRECTORY_TYPE 2
#define PATH_MAX 4096

static void take_action(char *filepath);
static int overwrite_file(char *filepath);
static int delete_file(char *filepath);
static int delete_directory(char *filepath);
static int get_file_type(char *filepath);
static void handle_error(const char *message, const char *filepath);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        handle_error("This command only accepts one arguments.", NULL);
        exit(1);
    }

    take_action(argv[1]);

    exit(0);
}

static void handle_error(const char *message, const char *filepath)
{
    if (filepath)
    {
        fprintf(stderr, "%s: %s\n", message, filepath);
    }
    else
    {
        fprintf(stderr, "%s\n", message);
    }
    exit(1);
}

static void take_action(char *filepath)
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
        handle_error("Invalid file type", filepath);
        exit(1);
    }
}

// Returns 0 on success
static int overwrite_file(char *filepath)
{
    FILE *file;
    long file_size;

    // Check if file exists
    if (!(file = fopen(filepath, "rb+")))
    {
        handle_error("File does not exist", filepath);
        return 1;
    }

    // Get size of file
    fseek(file, 0, SEEK_END);

    if ((file_size = ftell(file)) == -1L)
    {
        handle_error("Error determining file size", filepath);
        fclose(file);
        return 1;
    }
    else if (!file_size)
    {
        handle_error("Invalid file size", filepath);
        fclose(file);
        return 1;
    }
    fseek(file, 0, SEEK_SET);

    // Replace file contents
    char *overwrite_value = malloc(file_size);
    if (!overwrite_value)
    {
        handle_error("Failed to allocate memory for overwrite", filepath);
        free(overwrite_value);
        fclose(file);
        return 1;
    }

    // Pass 1: Overwrite with zeros
    memset(overwrite_value, 0, file_size);
    if (!fwrite(overwrite_value, file_size, 1, file))
    {
        handle_error("Error overwriting file", filepath);
        free(overwrite_value);
        fclose(file);
        return 1;
    }

    // Pass 2: Overwrite with binary ones
    memset(overwrite_value, 0xFF, file_size); 
    if (!fwrite(overwrite_value, file_size, 1, file))
    {
        handle_error("Error overwriting file", filepath);
        free(overwrite_value);
        fclose(file);
        return 1;
    }

    // Pass 3: Overwrite with random data
    for (long i = 0; i < file_size; i++)
    {
        overwrite_value[i] = (char)rand();
    }
    if (!fwrite(overwrite_value, file_size, 1, file))
    {
        handle_error("Error overwriting file", filepath);
        free(overwrite_value);
        fclose(file);
        return 1;
    }

    // Clear resources
    free(overwrite_value);
    fclose(file);
    
    return 0;
}

static int delete_file(char *filepath)
{
    if (overwrite_file(filepath))
    {
        handle_error("Terminating program, unable to overwrite", filepath);
        exit(1);
    }

    if (remove(filepath))
    {
        handle_error("Error deleting file", filepath);
        return 1;
    }

    printf("File deleted successfully: %s\n", filepath);
    return 0;
}

static int delete_directory(char *filepath)
{
    DIR *directory;
    struct dirent *directory_obj;
    char fullpath[PATH_MAX];

    // Check if directory exists
    if (!(directory = opendir(filepath)))
    {
        handle_error("Invalid directory", filepath);
        return 1;
    }

    while ((directory_obj = readdir(directory)))
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
    if (rmdir(filepath))
    {
        handle_error("Error deleting directory", filepath);
        return 1;
    }

    fprintf(stdout, "Directory deleted successfully: %s\n", filepath);
    return 0;
}

static int get_file_type(char *filepath)
{
    struct stat file_info;
    if (!stat(filepath, &file_info))
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