#ifndef CBREW_H
#define CBREW_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>

#define CBREW_TRUE 1
#define CBREW_FALSE 0

typedef uint8_t CbrewBool;

#define CBREW_SCOPED_MACRO(body) do { body } while(CBREW_FALSE)

/* Compiler detection */

#ifdef __GNUC__
#define CBREW_COMPILER "gcc"
#elif defined(__clang__)
#define CBREW_COMPILER "clang"
#else
#error "Unsupported C compiler!"
#endif

/* Platform detection */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define CBREW_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined(__linux__) || defined(__gnu_linux__)
#define CBREW_PLATFORM_LINUX
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#if defined(__ANDROID__)
#define CBREW_PLATFORM_ANDROID
#endif
#elif defined(__unix__)
#define CBREW_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
#define CBREW_PLATFORM_POSIX
#elif __APPLE__
#define CBREW_PLATFORM_APPLE
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define CBREW_PLATFORM_IOS
#define CBREW_PLATFORM_IOS_SIMULATOR
#elif TARGET_OS_IPHONE
#define CBREW_PLATFORM_IOS
#elif TARGET_OS_MAC
#define CBREW_PLATFORM_MAC_OS
#else
#error "Unknown Apple platform!"
#endif
#else
#error "Unknown platform!"
#endif

/* Architecture detection */

#if defined(__x86_64__) || defined(_M_X64)
#define CBREW_ARCHITECTURE_NAME "x86_64"
#else
#define CBREW_ARCHITECTURE_NAME "Unknown"
#endif

/* Platform dependencies */

#ifdef CBREW_PLATFORM_WINDOWS
#define CBREW_PLATFORM_NAME "Windows"
#define CBREW_PATH_SEPARATOR '\\'
#define CBREW_PATH_SEPARATOR_STR "\\"
#define CBREW_FILENAME_MAX 256
#define CBREW_FILEPATH_MAX MAX_PATH
#define CBREW_STATIC_LIB_EXTENSION "lib"
#define CBREW_DYNAMIC_LIB_EXTENSION "dll"
#define CBREW_LIB_PREFIX ""

#elif defined(CBREW_PLATFORM_LINUX)
#define CBREW_PLATFORM_NAME "Linux"
#define CBREW_PATH_SEPARATOR '/'
#define CBREW_PATH_SEPARATOR_STR "/"
#define CBREW_FILENAME_MAX 256
#define CBREW_FILEPATH_MAX PATH_MAX
#define CBREW_STATIC_LIB_EXTENSION "a"
#define CBREW_DYNAMIC_LIB_EXTENSION "so"
#define CBREW_LIB_PREFIX "lib"
#endif

#define CBREW_COMMAND_LENGTH_MAX 8192

#define CBREW_SOURCE_FILE "cbrew" CBREW_PATH_SEPARATOR_STR "cbrew.c"
#define CBREW_OLD_NAME "cbrew" CBREW_PATH_SEPARATOR_STR "cbrew.old"

/* Console colors */

#ifdef CBREW_ENABLE_CONSOLE_COLORS
#define CBREW_CONSOLE_COLOR_BLACK "\x1b[30m"
#define CBREW_CONSOLE_COLOR_DARK_BLUE "\x1b[34m"
#define CBREW_CONSOLE_COLOR_DARK_GREEN "\x1b[32m"
#define CBREW_CONSOLE_COLOR_DARK_AQUA "\x1b[36m"
#define CBREW_CONSOLE_COLOR_DARK_RED "\x1b[31m"
#define CBREW_CONSOLE_COLOR_DARK_PURPLE "\x1b[35m"
#define CBREW_CONSOLE_COLOR_GOLD "\x1b[33m"
#define CBREW_CONSOLE_COLOR_GRAY "\x1b[37m"
#define CBREW_CONSOLE_COLOR_DARK_GRAY "\x1b[90m"
#define CBREW_CONSOLE_COLOR_BLUE "\x1b[94m"
#define CBREW_CONSOLE_COLOR_GREEN "\x1b[92m"
#define CBREW_CONSOLE_COLOR_AQUA "\x1b[96m"
#define CBREW_CONSOLE_COLOR_RED "\x1b[91m"
#define CBREW_CONSOLE_COLOR_LIGHT_PURPLE "\x1b[95m"
#define CBREW_CONSOLE_COLOR_YELLOW "\x1b[93m"
#define CBREW_CONSOLE_COLOR_WHITE "\x1b[97m"
#define CBREW_CONSOLE_COLOR_RESET "\x1b[0m"
#else
#define CBREW_CONSOLE_COLOR_BLACK ""
#define CBREW_CONSOLE_COLOR_DARK_BLUE ""
#define CBREW_CONSOLE_COLOR_DARK_GREEN ""
#define CBREW_CONSOLE_COLOR_DARK_AQUA ""
#define CBREW_CONSOLE_COLOR_DARK_RED ""
#define CBREW_CONSOLE_COLOR_DARK_PURPLE ""
#define CBREW_CONSOLE_COLOR_GOLD ""
#define CBREW_CONSOLE_COLOR_GRAY ""
#define CBREW_CONSOLE_COLOR_DARK_GRAY ""
#define CBREW_CONSOLE_COLOR_BLUE ""
#define CBREW_CONSOLE_COLOR_GREEN ""
#define CBREW_CONSOLE_COLOR_AQUA ""
#define CBREW_CONSOLE_COLOR_RED ""
#define CBREW_CONSOLE_COLOR_LIGHT_PURPLE ""
#define CBREW_CONSOLE_COLOR_YELLOW ""
#define CBREW_CONSOLE_COLOR_WHITE ""
#define CBREW_CONSOLE_COLOR_RESET ""
#endif

/* Logging colors */

#ifndef CBREW_CONSOLE_COLOR_ERROR
#define CBREW_CONSOLE_COLOR_ERROR CBREW_CONSOLE_COLOR_RED
#endif

#ifndef CBREW_CONSOLE_COLOR_WARN
#define CBREW_CONSOLE_COLOR_WARN CBREW_CONSOLE_COLOR_YELLOW
#endif

#ifndef CBREW_CONSOLE_COLOR_INFO
#define CBREW_CONSOLE_COLOR_INFO CBREW_CONSOLE_COLOR_DARK_GREEN
#endif

#ifndef CBREW_CONSOLE_COLOR_TRACE
#define CBREW_CONSOLE_COLOR_TRACE CBREW_CONSOLE_COLOR_GRAY
#endif

#ifndef CBREW_CONSOLE_COLOR_COMMAND
#define CBREW_CONSOLE_COLOR_COMMAND CBREW_CONSOLE_COLOR_AQUA
#endif

#ifndef CBREW_CONSOLE_COLOR_PROJECT
#define CBREW_CONSOLE_COLOR_PROJECT CBREW_CONSOLE_COLOR_WHITE
#endif

#ifndef CBREW_CONSOLE_COLOR_CONFIG
#define CBREW_CONSOLE_COLOR_CONFIG CBREW_CONSOLE_COLOR_WHITE
#endif

#ifndef CBREW_CONSOLE_COLOR_TIME
#define CBREW_CONSOLE_COLOR_TIME CBREW_CONSOLE_COLOR_DARK_GRAY
#endif

/* Logging */

#define CBREW_LOG_ERROR(format, ...) CBREW_SCOPED_MACRO(printf("%s[ERROR]: ", CBREW_CONSOLE_COLOR_ERROR); printf(format, ##__VA_ARGS__); printf("%s\n", CBREW_CONSOLE_COLOR_RESET);)
#define CBREW_LOG_WARN(format, ...) CBREW_SCOPED_MACRO(printf("%s[WARN]: ", CBREW_CONSOLE_COLOR_WARN); printf(format, ##__VA_ARGS__); printf("%s\n", CBREW_CONSOLE_COLOR_RESET);)
#define CBREW_LOG_INFO(format, ...) CBREW_SCOPED_MACRO(printf("%s[INFO]: ", CBREW_CONSOLE_COLOR_INFO); printf(format, ##__VA_ARGS__); printf("%s\n", CBREW_CONSOLE_COLOR_RESET);)
#define CBREW_LOG_TRACE(format, ...) CBREW_SCOPED_MACRO(printf("%s[TRACE]: ", CBREW_CONSOLE_COLOR_TRACE); printf(format, ##__VA_ARGS__); printf("%s\n", CBREW_CONSOLE_COLOR_RESET);)

#ifdef CBREW_LOG_COMMANDS
#define CBREW_LOG_CMD(format, ...) CBREW_SCOPED_MACRO(printf("%s[CMD]: ", CBREW_CONSOLE_COLOR_COMMAND); printf(format, ##__VA_ARGS__); printf("%s\n", CBREW_CONSOLE_COLOR_RESET);)
#else
#define CBREW_LOG_CMD(format, ...)
#endif

/* Asserts */

#ifdef CBREW_ENABLE_ASSERTS
#define CBREW_ASSERT(condition) CBREW_SCOPED_MACRO(if(!(condition)) { printf("%s[ERROR]: Assertion failed: %s: %s\n", CBREW_CONSOLE_COLOR_ERROR, __FUNCTION__, #condition); exit(EXIT_FAILURE); })
#else
#define CBREW_ASSERT(condition)
#endif

/* Shortcut macros */

#define CBREW_CMD(format, ...) cbrew_command(format, ##__VA_ARGS__)

#define CBREW_PRJ_NEW(name, type) cbrew_project_create(name, type)
#define CBREW_PRJ_FILES(prj, files) cbrew_project_add_files(prj, files)
#define CBREW_PRJ_INCLUDE_DIR(prj, include_dir) cbrew_project_add_include_dir(prj, include_dir)
#define CBREW_PRJ_DEFINE(prj, define) cbrew_project_add_define(prj, define)
#define CBREW_PRJ_FLAG(prj, flag) cbrew_project_add_flag(prj, flag)
#define CBREW_PRJ_LINK(prj, link) cbrew_project_add_link(prj, link)

#define CBREW_CFG_NEW(prj, name, target_dir, obj_dir) cbrew_project_config_create(prj, name, target_dir, obj_dir)
#define CBREW_CFG_DEFINE(cfg, define) cbrew_config_add_define(cfg, define)
#define CBREW_CFG_FLAG(cfg, flag) cbrew_config_add_flag(cfg, flag)

/*
* This macro can be used if you compile CBREW with only one source file - as intended.
* When used, everytime you run CBREW it will check when you last modified
* your source file and if you modified it after the CBREW executable
* was created, than CBREW will automatically rebuild itself.
*/
#define CBREW_AUTO_REBUILD(argc, argv)\
    CBREW_SCOPED_MACRO(\
        if(!cbrew_file_exists(CBREW_SOURCE_FILE))\
        {\
            CBREW_LOG_WARN("%s does not exist!", CBREW_SOURCE_FILE);\
            break;\
        }\
        \
        char old_name[CBREW_FILEPATH_MAX];\
        cbrew_executable_filepath(old_name, argv[0]);\
        \
        if(!cbrew_first_file_is_older(old_name, CBREW_SOURCE_FILE))\
        {\
            break;\
        }\
        \
        CBREW_LOG_TRACE("Rebuilding...");\
        \
        if(!cbrew_file_rename(old_name, CBREW_OLD_NAME))\
        {\
            CBREW_LOG_ERROR("Failed to rename old executable!");\
            exit(EXIT_FAILURE);\
        }\
        \
        if(!cbrew_command("%s -Wall -Wextra -O3 -o cbrew%ccbrew %s", CBREW_COMPILER, CBREW_PATH_SEPARATOR, CBREW_SOURCE_FILE))\
        {\
            CBREW_LOG_ERROR("Failed to rebuild!");\
            cbrew_file_rename(CBREW_OLD_NAME, old_name);\
            exit(EXIT_FAILURE);\
        }\
        \
        if(!cbrew_self_destruct())\
            CBREW_LOG_WARN("Failed to remove old executable!");\
        \
        CBREW_LOG_INFO("Successfully rebuilt");\
        \
        if(!cbrew_command(old_name))\
        {\
            CBREW_LOG_ERROR("Failed to restart!");\
            exit(EXIT_FAILURE);\
        }\
        \
        exit(EXIT_SUCCESS);\
    )

typedef struct CbrewConfig
{
    const char* name;

    const char* target_dir;
    const char* obj_dir;

    char** defines;
    size_t defines_count;

    char** flags;
    size_t flags_count;
} CbrewConfig;

enum
{
    CBREW_PROJECT_TYPE_APP = 0,
    CBREW_PROJECT_TYPE_STATIC_LIB,
    CBREW_PROJECT_TYPE_DYNAMIC_LIB,

    CBREW_PROJECT_TYPE_SIZE
};

typedef uint8_t CbrewProjectType;

typedef struct CbrewProject
{
    const char* name;
    CbrewProjectType type;

    char** wildcards;
    size_t wildcards_count;

    char** include_dirs;
    size_t include_dirs_count;

    char** defines;
    size_t defines_count;

    char** flags;
    size_t flags_count;

    char** links;
    size_t links_count;

    CbrewConfig* configs;
    size_t configs_count;
} CbrewProject;

/**
* Builds all projects with all configurations that have been registered with cbrew.
* @param argc argc of the main() function.
* @param argv argv of the main() function.
*/
void cbrew_build(int argc, char** argv);

/* Projects */

/**
* Creates a new project.
* @param name The name of the project.
* @param type The type of the project.
* @return Returns a pointer to the newly created project.
*/
CbrewProject* cbrew_project_create(const char* name, CbrewProjectType type);

/**
* Adds files to the project.
* @param project The project to which the file will be added.
* @param wildcard The wildcard that must be matched for a file to be added to the project.
*/
void cbrew_project_add_files(CbrewProject* project, const char* wildcard);

/**
* Adds a flag to the project.
* @param project The project to which the flag will be added.
* @param flag The flag to add, which must include the single or double hyphen.
*/
void cbrew_project_add_flag(CbrewProject* project, const char* flag);

/**
* Adds a define to the project.
* @param project The project to which the define will be added.
* @param define The define to add.
*/
void cbrew_project_add_define(CbrewProject* project, const char* define);

/**
* Adds an include directory to the project.
* @param project The project to which the include directory will be added.
* @param include_dir The path to the include directory.
*/
void cbrew_project_add_include_dir(CbrewProject* project, const char* include_dir);

/**
* Adds a link (e.g. libraries) to the project.
* @param project The project to which the link will be added.
* @param link The path to the link.
*/
void cbrew_project_add_link(CbrewProject* project, const char* link);

/**
* Checks whether or not the specified filepath is a source file that belongs to the specified project.
* @param project The project whose wildcards are used to match against the filepath.
* @param filepath The filepath which is matched against the project's wildcards.
* @return Returns CBREW_TRUE if the filepath matches any of the project's wildcards, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_project_file_matches_wildcards(const CbrewProject* project, const char* filepath);

/**
* Constructs a string array consisting of all source files for the specified project.
* @param project The project whose source files will be found.
* @param files_count A pointer whose value will be set to the amount of source files found. If NULL, no count will be set.
* @return Returns a string array of the found source files or NULL if no source files were found. The individual strings as well as the array itself needs to be freed by the caller.
*/
char** cbrew_project_find_source_files(const CbrewProject* project, size_t* files_count);

/**
* Builds the project with every config of the project, which includes compilation and linkage.
* @param project The project to build.
* @return Returns CBREW_TRUE on success, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_project_build(const CbrewProject* project);

/**
* Checks whether an object file alreadys exists for a given source file and checks whether or not the object file is still up to date.
* @param project The project that is compiled.
* @param config The config that is compiled with.
* @param filepath The filepath of the source file.
* @return Returns CBREW_TRUE if there is already an up to date object file for the specified source file, CBREW_FALSE otherwise or if the executable was rebuilt after the compilation of the object file.
*/
CbrewBool cbrew_project_config_file_is_already_compiled(const CbrewProject* project, const CbrewConfig* config, const char* filepath);

/**
* Compiles a project with the specified config into object files.
* @param project The project to build.
* @param config The config to build the project with.
* @param project_files A string array containing the source files of the project.
* @param project_files_count The count of the project's source files.
* @return Returns CBREW_TRUE on success, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_project_config_compile(const CbrewProject* project, const CbrewConfig* config, char** project_files, size_t project_files_count);

/**
* Compiles a project with the specified config into a static library.
* @param project The project to build.
* @param config The config to build the project with.
* @return Returns CBREW_TRUE on success, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_project_config_compile_static_lib(const CbrewProject* project, const CbrewConfig* config);

/**
* Compiles a project with the specified config into a dynamic library.
* @param project The project to build.
* @param config The config to build the project with.
* @return Returns CBREW_TRUE on success, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_project_config_compile_dynamic_lib(const CbrewProject* project, const CbrewConfig* config);

/**
* Links the object files of the specified config into an executable.
* @param project The project to build.
* @param config The config to build the project with.
* @return Returns CBREW_TRUE on success, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_project_config_link(const CbrewProject* project, const CbrewConfig* config);

/* Configs */

/**
* Creates a new config for a given project.
* @param project The project for which the config is created.
* @param name The name of the config.
* @param target_dir The path to the target directory of the config.
* @param obj_dir The path to the object directory of the config.
* @return Returns a pointer to the newly created config.
*/
CbrewConfig* cbrew_project_config_create(CbrewProject* project, const char* name, const char* target_dir, const char* obj_dir);

/**
* Adds a flag to the config.
* @param config The config to which the flag will be added.
* @param flag The flag to add, which must include the single or double hyphen.
*/
void cbrew_config_add_flag(CbrewConfig* config, const char* flag);

/**
* Adds a define to the config.
* @param config The config to which the define will be added.
* @param define The define to add.
*/
void cbrew_config_add_define(CbrewConfig* config, const char* define);

/**
* Creates a string which contains all object files inside the config's object directory.
* @param config The config of which the object files inside the object directory should be retrieved.
* @return Returns a string which contains all object files inside the config's object directory. This string must be freed by the caller.
*/
char* cbrew_config_create_obj_files_str(const CbrewConfig* config);

/* Util */

/**
* Converts any string to a valid path string for the current system by replacing every '/' character with the CBREW_PATH_SEPARATOR character.
* @param path The path to convert.
* @return Returns the modified string.
*/
char* cbrew_path(char* path);

/**
* Creates a string which contains all elements of the specicfied array.
* The individual strings are separated by a whitespace.
* @param flags The string array containing the flags.
* @param flags_count The element count of flags.
* @return Returns a string which contains all elements of flags. This string must be freed by the caller.
*/
char* cbrew_create_flags_str(char** flags, size_t flags_count);

/**
* Creates a string which contains all elements of the specicfied array.
* "-D" will be inserted before every define.
* The individual strings are separated by a whitespace.
* @param defines The string array containing the defines.
* @param defines_count The element count of defines.
* @return Returns a string which contains all elements of defines. This string must be freed by the caller.
*/
char* cbrew_create_defines_str(char** defines, size_t defines_count);

/**
* Creates a string which contains all elements of the specicfied array.
* "-L" will be inserted before every include directory.
* The individual strings are separated by a whitespace.
* @param include_dirs The string array containing the include_dirs.
* @param include_dirs_count The element count of include_dirs.
* @return Returns a string which contains all elements of include_dirs. This string must be freed by the caller.
*/
char* cbrew_create_include_dirs_str(char** include_dirs, size_t include_dirs_count);

/**
* Creates a string which contains all elements of the specicfied array.
* "-l" will be inserted before every link.
* The individual strings are separated by a whitespace.
* @param links The string array containing the links.
* @param links_count The element count of links.
* @return Returns a string which contains all elements of links. This string must be freed by the caller.
*/
char* cbrew_create_links_str(char** links, size_t links_count);

/* IO */

/**
* Copies the filepath inside argv[0] into filepath with the correct extension, if the extension of the executable is not already inside argv[0].
* @param filepath A string large enough to store the filepath.
*/
void cbrew_executable_filepath(char* filepath, const char* argv0);

/**
* Executes a command on the command prompt.
* @param format The command or command format.
* @return Returns CBREW_TRUE if the command was executed and returned EXIT_SUCCESS, CBREW_FALSE oherwise.
*/
CbrewBool cbrew_command(const char* format, ...);

/**
* Checks if the last modification time of the first file was before the last modification time of the second file.
* @param first_file The first file.
* @param second_file The second file.
* @return Returns CBREW_TRUE if first_file is older than second_file, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_first_file_is_older(const char* first_file, const char* second_file);

/**
* Checks if a file exists.
* @param file The path of the file.
* @return Returns CBREW_TRUE when the file exists, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_file_exists(const char* file);

/**
* Renames a file. If a file with the new name already exists, it will be overwritten.
* @param old_name The path of the file which name shall be changed.
* @param new_name The path that the file shall be renamed to.
* @return Returns CBREW_TRUE when the file could be renamed, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_file_rename(const char* old_name, const char* new_name);

/**
* Copies a file.
* @param src_path The filepath to the file that is to be copied.
* @param dest_path The filepath to which the file will be copied.
* @return Returns CBREW_TRUE if the file was copied successfully, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_file_copy(const char* src_path, const char* dest_path);

/**
* Deletes a file.
* @param file The path of the file to delete.
* @return Returns CBREW_TRUE if the file could be deleted, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_file_delete(const char* file);

/**
* Retrieves the directory for a given wildcard.
* @param dir The string to hold the retrieved directory.
* @param wildcard The wildcard to retrieve the directory from.
*/
void cbrew_wildcard_directory(char* dir, const char* wildcard);

/**
 * Checks whether or not a filepath matches a given wildcard.
 * @param filepath The filepath to match against.
 * @param wildcard The wildcard that will be matched against.
 * @return Returns CBREW_TRUE if the filepath matches the wildcard, CBREW_FALSE otherwise.
 */
CbrewBool cbrew_file_matches_wildcard(const char* filepath, const char* wildcard);

/**
* Scans a directory for all files and returns an array of strings containing the filenames.
* @param dir The directory to scan for files.
* @param files_count A pointer whose value will be set to the amount of files found. If NULL, no count will be set.
* @return Returns a string array of the found files or NULL if no files were found. The individual strings as well as the array itself needs to be freed by the caller.
*/
char** cbrew_find_files(const char* dir, size_t* files_count);

/**
* Scans a directory and all subdirectories for all files and returns an array of strings containing the filenames.
* @param dir The directory to scan for files.
* @param files_count A pointer whose value will be set to the amount of files found. If NULL, no count will be set.
* @return Returns a string array of the found files or NULL if no files were found. The individual strings as well as the array itself needs to be freed by the caller.
*/
char** cbrew_find_files_recursive(const char* dir, size_t* files_count);

/**
* Checks if a directory exists.
* @param dir The path of the directory.
* @return Returns CBREW_TRUE if the directory exists, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_dir_exists(const char* dir);

/**
* Creates a new directory.
* @param dir The path of the directory which will be created.
* @return Returns CBREW_TRUE if the directory could be created, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_dir_create(const char* dir);

/**
* Self destructs the executable.
* @return Returns CBREW_TRUE on success, CBREW_FALSE otherwise.
*/
CbrewBool cbrew_self_destruct(void);

#ifdef CBREW_IMPLEMENTATION

typedef struct CbrewHandler
{
    CbrewProject* projects;
    size_t projects_count;

    const char* argv0;
} CbrewHandler;

static CbrewHandler handler;

void cbrew_build(int argc, char** argv)
{
    (void)argc;

    const clock_t start = clock();

    handler.argv0 = argv[0];

    CbrewBool success = CBREW_TRUE;

    for(size_t i = 0; i < handler.projects_count; ++i)
    {
        CbrewProject* project = &handler.projects[i];
        if(!cbrew_project_build(project))
            success = CBREW_FALSE;
    }

    const clock_t end = clock();

    const double building_time = (((double) end - start) / CLOCKS_PER_SEC) * 1000;

    if(success)
        CBREW_LOG_INFO("Successfully built all projects %s(%.0lfms)", CBREW_CONSOLE_COLOR_TIME, building_time);
    else
        CBREW_LOG_ERROR("Build process finished with errors! %s(%.0lfms)", CBREW_CONSOLE_COLOR_TIME, building_time);
}

/* Projects */

CbrewProject* cbrew_project_create(const char* name, CbrewProjectType type)
{
    CBREW_ASSERT(name != NULL);
    CBREW_ASSERT(type < CBREW_PROJECT_TYPE_SIZE);
    
    for(size_t i = 0; i < handler.projects_count; ++i)
    {
        if(strcmp(handler.projects[i].name, name) == 0)
        {
            CBREW_LOG_WARN("Project with name %s%s%s already exists!", CBREW_CONSOLE_COLOR_PROJECT, name, CBREW_CONSOLE_COLOR_WARN);
            return &handler.projects[i];
        }
    }

    ++handler.projects_count;
    handler.projects = realloc(handler.projects, handler.projects_count * sizeof(CbrewProject));

    CbrewProject* project = &handler.projects[handler.projects_count - 1];

    memset(project, 0, sizeof(CbrewProject));

    project->name = name;
    project->type = type;

    return project;
}

void cbrew_project_add_files(CbrewProject* project, const char* wildcard)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(wildcard != NULL);
    
    ++project->wildcards_count;
    project->wildcards = realloc(project->wildcards, project->wildcards_count * sizeof(char*));
    project->wildcards[project->wildcards_count - 1] = cbrew_path(strdup(wildcard));
}

void cbrew_project_add_flag(CbrewProject* project, const char* flag)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(flag != NULL);
    
    ++project->flags_count;
    project->flags = realloc(project->flags, project->flags_count * sizeof(char*));
    project->flags[project->flags_count - 1] = strdup(flag);
}

void cbrew_project_add_define(CbrewProject* project, const char* define)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(define != NULL);

    ++project->defines_count;
    project->defines = realloc(project->defines, project->defines_count * sizeof(char*));
    project->defines[project->defines_count - 1] = strdup(define);
}

void cbrew_project_add_include_dir(CbrewProject* project, const char* include_dir)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(include_dir != NULL);
    
    ++project->include_dirs_count;
    project->include_dirs = realloc(project->include_dirs, project->include_dirs_count * sizeof(char*));
    project->include_dirs[project->include_dirs_count - 1] = cbrew_path(strdup(include_dir));
}

void cbrew_project_add_link(CbrewProject* project, const char* link)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(link != NULL);
    
    ++project->links_count;
    project->links = realloc(project->links, project->links_count * sizeof(char*));
    project->links[project->links_count - 1] = cbrew_path(strdup(link));
}

CbrewBool cbrew_project_file_matches_wildcards(const CbrewProject* project, const char* filepath)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(filepath != NULL);

    for(size_t i = 0; i < project->wildcards_count; ++i)
    {
        if(cbrew_file_matches_wildcard(filepath, project->wildcards[i]))
            return CBREW_TRUE;
    }

    return CBREW_FALSE;
}

char** cbrew_project_find_source_files(const CbrewProject* project, size_t* files_count)
{
    CBREW_ASSERT(project != NULL);

    size_t files_found_count = 0;
    char** files_found = cbrew_find_files_recursive(".", &files_found_count);

    size_t source_files_count = 0;
    for(size_t i = 0; i < files_found_count; ++i)
    {
        if(cbrew_project_file_matches_wildcards(project, files_found[i]))
            ++source_files_count;
    }

    if(files_count != NULL)
        *files_count = source_files_count;

    
    if(source_files_count == 0)
    {
        for(size_t i = 0; i < files_found_count; ++i)
            free(files_found[i]);

        free(files_found);

        return NULL;
    }

    char** source_files = malloc(source_files_count * sizeof(char*));

    size_t source_file_index = 0;
    for(size_t i = 0; i < files_found_count; ++i)
    {
        if(cbrew_project_file_matches_wildcards(project, files_found[i]))
            source_files[source_file_index++] = files_found[i];
        else
            free(files_found[i]);
    }

    free(files_found);

    return source_files;
}

CbrewBool cbrew_project_build(const CbrewProject* project)
{
    CBREW_ASSERT(project != NULL);

    const clock_t project_start = clock();

    CBREW_LOG_TRACE("Building project %s%s%s...", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_TRACE);

    if(project->configs_count == 0)
    {
        CBREW_LOG_WARN("Project %s%s %shas no configs!", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_WARN);
        CBREW_LOG_WARN("Skipping project %s%s%s!", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_WARN);
        return CBREW_TRUE;
    }

    size_t project_files_count = 0;
    char** project_files = cbrew_project_find_source_files(project, &project_files_count);

    if(project_files_count == 0)
    {
        CBREW_LOG_WARN("Project %s%s %shas no source files!", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_WARN);
        CBREW_LOG_WARN("Skipping project %s%s%s!", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_WARN);
        return CBREW_TRUE;
    }

    if(project->type >= CBREW_PROJECT_TYPE_SIZE)
    {
        CBREW_LOG_WARN("Invalid project type specified for project %s%s%s!", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_WARN);
        CBREW_LOG_WARN("Skipping project %s%s%s!", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_WARN);
        
        for(size_t i = 0; i < project_files_count; ++i)
            free(project_files[i]);

        free(project_files);
        
        return CBREW_FALSE;
    }

    CbrewBool project_success = CBREW_TRUE;

    for(size_t i = 0; i < project->configs_count; ++i)
    {
        CbrewConfig* config = &project->configs[i];

        const clock_t config_start = clock();

        CBREW_LOG_TRACE("Building config %s%s%s...", CBREW_CONSOLE_COLOR_CONFIG, config->name, CBREW_CONSOLE_COLOR_TRACE);

        CbrewBool config_success = CBREW_TRUE;

        if(cbrew_project_config_compile(project, config, project_files, project_files_count))
        {
            switch(project->type)
            {
                case CBREW_PROJECT_TYPE_APP:
                {
                    if(!cbrew_project_config_link(project, config))
                    {
                        CBREW_LOG_ERROR("Failed to create executable for config %s%s%s!", CBREW_CONSOLE_COLOR_CONFIG, config->name, CBREW_CONSOLE_COLOR_ERROR);
                        project_success = CBREW_FALSE;
                        config_success = CBREW_FALSE;
                    }

                    break;
                }

                case CBREW_PROJECT_TYPE_STATIC_LIB:
                {
                    if(!cbrew_project_config_compile_static_lib(project, config))
                    {
                        CBREW_LOG_ERROR("Failed to create Static Lib for config %s%s%s!", CBREW_CONSOLE_COLOR_CONFIG, config->name, CBREW_CONSOLE_COLOR_ERROR);
                        project_success = CBREW_FALSE;
                        config_success = CBREW_FALSE;
                    }

                    break;
                }

                case CBREW_PROJECT_TYPE_DYNAMIC_LIB:
                {
                    if(!cbrew_project_config_compile_dynamic_lib(project, config))
                    {
                        CBREW_LOG_ERROR("Failed to create Dynamic Lib for config %s%s%s!", CBREW_CONSOLE_COLOR_CONFIG, config->name, CBREW_CONSOLE_COLOR_ERROR);
                        project_success = CBREW_FALSE;
                        config_success = CBREW_FALSE;
                    }

                    break;
                }

                default: CBREW_ASSERT(CBREW_FALSE);
            }
        }
        else
        {
            CBREW_LOG_ERROR("Failed to compile source files for config %s%s%s!", CBREW_CONSOLE_COLOR_CONFIG, config->name, CBREW_CONSOLE_COLOR_ERROR);
            project_success = CBREW_FALSE;
            config_success = CBREW_FALSE;
        }

        const clock_t config_end = clock();
        const double config_building_time = (((double) config_end - config_start) / CLOCKS_PER_SEC) * 1000;

        if(config_success)
            CBREW_LOG_INFO("Successfully built config %s%s %s(%.0lfms)", CBREW_CONSOLE_COLOR_CONFIG, config->name, CBREW_CONSOLE_COLOR_TIME, config_building_time);
        else
            CBREW_LOG_ERROR("Failed to build config %s%s%s!", CBREW_CONSOLE_COLOR_CONFIG, config->name, CBREW_CONSOLE_COLOR_ERROR);
    }

    for(size_t i = 0; i < project_files_count; ++i)
        free(project_files[i]);

    free(project_files);

    const clock_t project_end = clock();
    const double project_building_time = (((double) project_end - project_start) / CLOCKS_PER_SEC) * 1000;

    if(project_success)
    {
        CBREW_LOG_INFO("Successfully built project %s%s %s(%.0lfms)", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_TIME, project_building_time);
        return CBREW_TRUE;
    }
    
    CBREW_LOG_ERROR("Failed to build project %s%s%s!", CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_ERROR);
    return CBREW_FALSE;
}

CbrewBool cbrew_project_config_file_is_already_compiled(const CbrewProject* project, const CbrewConfig* config, const char* filepath)
{   
    const char* filename = strrchr(filepath, CBREW_PATH_SEPARATOR);
    filename = filename == NULL ? filepath : filename + 1;
        
    char obj_filepath[CBREW_FILEPATH_MAX];
    sprintf(obj_filepath, "%s%c%s", config->obj_dir, CBREW_PATH_SEPARATOR, filename);

    char* extension = strrchr(obj_filepath, '.');
    if(extension != NULL)
    {
        extension[1] = 'o';
        extension[2] = '\0';
    }
    else
    {   
        size_t obj_filepath_len = strlen(obj_filepath);
        obj_filepath[obj_filepath_len] = '.';
        obj_filepath[obj_filepath_len + 1] = 'o';
        obj_filepath[obj_filepath_len + 2] = '\0';
    }

    if(!cbrew_file_exists(obj_filepath))
        return CBREW_FALSE;

    char executable[CBREW_FILEPATH_MAX];
    cbrew_executable_filepath(executable, handler.argv0);

    if(cbrew_first_file_is_older(obj_filepath, executable))
        return CBREW_FALSE;

    char* include_dirs = cbrew_create_include_dirs_str(project->include_dirs, project->include_dirs_count);
    char dependency_command[strlen(CBREW_COMPILER) + strlen(" ") + strlen(include_dirs) + strlen(" -MM ") + CBREW_FILEPATH_MAX];
    sprintf(dependency_command, "%s %s-MM %s", CBREW_COMPILER, include_dirs, filepath);
    free(include_dirs);

    FILE* command_pipe = popen(dependency_command, "r");

    if(command_pipe == NULL)
        return CBREW_FALSE;

    char dependency_str[256 * CBREW_FILEPATH_MAX];
    if(fgets(dependency_str, sizeof(dependency_str), command_pipe) == NULL)
    {
        pclose(command_pipe);
        return CBREW_FALSE;
    }

    dependency_str[strlen(dependency_str) - 1] = '\0';

    char* dependency = strtok(dependency_str, " ");
    while((dependency = strtok(NULL, " ")) != NULL)
    {
        cbrew_path(dependency);
        
        if(cbrew_first_file_is_older(obj_filepath, dependency))
        {
            pclose(command_pipe);

            return CBREW_FALSE;
        }
    }

    pclose(command_pipe);

    return CBREW_TRUE;
}

CbrewBool cbrew_project_config_compile(const CbrewProject* project, const CbrewConfig* config, char** project_files, size_t project_files_count)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(config != NULL);
    CBREW_ASSERT(project_files != NULL);

    if(!cbrew_dir_exists(config->obj_dir))
        cbrew_dir_create(config->obj_dir);

    char* project_flags = cbrew_create_flags_str(project->flags, project->flags_count);
    char* project_defines = cbrew_create_defines_str(project->defines, project->defines_count);
    char* include_dirs = cbrew_create_include_dirs_str(project->include_dirs, project->include_dirs_count);

    char* config_flags = cbrew_create_flags_str(config->flags, config->flags_count);
    char* config_defines = cbrew_create_defines_str(config->defines, config->defines_count);

    CbrewBool result = CBREW_TRUE;

    for(size_t i = 0; i < project_files_count; ++i)
    {
        CBREW_ASSERT(project_files[i] != NULL);

        const char* filename = strrchr(project_files[i], CBREW_PATH_SEPARATOR);
        filename = filename == NULL ? project_files[i] : filename + 1;

        if(cbrew_project_config_file_is_already_compiled(project, config, project_files[i]))
            continue;

        size_t filename_len = strlen(filename);
        char obj_file[CBREW_FILENAME_MAX];
        strcpy(obj_file, filename);
        char* extension = strrchr(obj_file, '.');
        if(extension != NULL)
        {
            extension[1] = 'o';
            extension[2] = '\0';
        }
        else
        {
            obj_file[filename_len] = '.';
            obj_file[filename_len + 1] = 'o';
            obj_file[filename_len + 2] = '\0';
        }

        result = cbrew_command("%s %s%s%s%s%s-c -o \"%s%c%s\" \"%s\"", CBREW_COMPILER, include_dirs, project_flags, config_flags, project_defines, config_defines, config->obj_dir, CBREW_PATH_SEPARATOR, obj_file, project_files[i]);

        if(!result)
            break;
    }

    free(project_flags);
    free(project_defines);
    free(include_dirs);

    free(config_flags);
    free(config_defines);

    return result;
}

CbrewBool cbrew_project_config_compile_static_lib(const CbrewProject* project, const CbrewConfig* config)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(config != NULL);

    char* obj_files = cbrew_config_create_obj_files_str(config);

    if(!cbrew_dir_exists(config->target_dir))
        cbrew_dir_create(config->target_dir);

    CbrewBool result = cbrew_command("ar rcs \"%s%c%s%s.%s\" %s", config->target_dir, CBREW_PATH_SEPARATOR, CBREW_LIB_PREFIX, project->name, CBREW_STATIC_LIB_EXTENSION, obj_files);

    free(obj_files);

    return result;
}

CbrewBool cbrew_project_config_compile_dynamic_lib(const CbrewProject* project, const CbrewConfig* config)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(config != NULL);

    char* obj_files = cbrew_config_create_obj_files_str(config);

    if(!cbrew_dir_exists(config->target_dir))
        cbrew_dir_create(config->target_dir);
    
    char* project_flags = cbrew_create_flags_str(project->flags, project->flags_count);
    char* config_flags = cbrew_create_flags_str(config->flags, config->flags_count);

    CbrewBool result = cbrew_command("%s -shared %s%s-o \"%s%c%s.%s\" %s", CBREW_COMPILER, project_flags, config_flags, config->target_dir, CBREW_PATH_SEPARATOR, project->name, CBREW_DYNAMIC_LIB_EXTENSION, obj_files);

    free(project_flags);
    free(config_flags);

    free(obj_files);

    return result;
}

CbrewBool cbrew_project_config_link(const CbrewProject* project, const CbrewConfig* config)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(config != NULL);

    char* obj_files = cbrew_config_create_obj_files_str(config);

    if(!cbrew_dir_exists(config->target_dir))
        cbrew_dir_create(config->target_dir);

    char* include_dirs = cbrew_create_include_dirs_str(project->include_dirs, project->include_dirs_count);

    char* project_flags = cbrew_create_flags_str(project->flags, project->flags_count);
    char* project_links = cbrew_create_links_str(project->links, project->links_count);

    char* config_flags = cbrew_create_flags_str(config->flags, config->flags_count);

    CbrewBool result = cbrew_command("%s %s%s%s-o \"%s%c%s\" %s %s", CBREW_COMPILER, include_dirs, project_flags, config_flags, config->target_dir, CBREW_PATH_SEPARATOR, project->name, obj_files, project_links);

    free(include_dirs);

    free(project_flags);
    free(project_links);

    free(config_flags);

    free(obj_files);

    return result;
}

/* Configs */

CbrewConfig* cbrew_project_config_create(CbrewProject* project, const char* name, const char* target_dir, const char* obj_dir)
{
    CBREW_ASSERT(project != NULL);
    CBREW_ASSERT(name != NULL);
    CBREW_ASSERT(target_dir != NULL);
    CBREW_ASSERT(obj_dir != NULL);

    for(size_t i = 0; i < project->configs_count; ++i)
    {
        if(strcmp(project->configs[i].name, name) == 0)
        {
            CBREW_LOG_WARN("Config with name %s%s%s already exists for project %s%s%s!", CBREW_CONSOLE_COLOR_CONFIG, name, CBREW_CONSOLE_COLOR_WARN, CBREW_CONSOLE_COLOR_PROJECT, project->name, CBREW_CONSOLE_COLOR_WARN);
            return &project->configs[i];
        }
    }

    ++project->configs_count;
    project->configs = realloc(project->configs, project->configs_count * sizeof(CbrewConfig));

    CbrewConfig* config = &project->configs[project->configs_count - 1];

    memset(config, 0, sizeof(CbrewConfig));

    config->name = name;
    config->target_dir = cbrew_path(strdup(target_dir));
    config->obj_dir = cbrew_path(strdup(obj_dir));

    return config;
}

void cbrew_config_add_flag(CbrewConfig* config, const char* flag)
{
    CBREW_ASSERT(config != NULL);
    CBREW_ASSERT(flag != NULL);
    
    ++config->flags_count;
    config->flags = realloc(config->flags, config->flags_count * sizeof(char*));
    config->flags[config->flags_count - 1] = strdup(flag);
}

void cbrew_config_add_define(CbrewConfig* config, const char* define)
{
    CBREW_ASSERT(config != NULL);
    CBREW_ASSERT(define != NULL);
    
    ++config->defines_count;
    config->defines = realloc(config->defines, config->defines_count * sizeof(char*));
    config->defines[config->defines_count - 1] = strdup(define);
}

char* cbrew_config_create_obj_files_str(const CbrewConfig* config)
{
    CBREW_ASSERT(config != NULL);
    
    char obj_file_wildcard[CBREW_FILEPATH_MAX];
    sprintf(obj_file_wildcard, "%s%c*.o", config->obj_dir, CBREW_PATH_SEPARATOR);

    size_t obj_files_count = 0;
    char** obj_files = cbrew_find_files(config->obj_dir, &obj_files_count);

    size_t obj_files_buffer_len = 0;
    for(size_t i = 0; i < obj_files_count; ++i)
    {
        if(!cbrew_file_matches_wildcard(obj_files[i], obj_file_wildcard))
            continue;

        obj_files_buffer_len += strlen("\"") + strlen(obj_files[i]) + strlen("\"") + strlen(" ");
    }

    if(obj_files_buffer_len == 0)
    {
        CBREW_LOG_WARN("Config %s%s%s has no object files!", CBREW_CONSOLE_COLOR_CONFIG, config->name, CBREW_CONSOLE_COLOR_WARN);

        for(size_t i = 0; i < obj_files_count; ++i)
            free(obj_files[i]);

        free(obj_files);

        return calloc(1, sizeof(char));
    }
                
    char* obj_files_buffer = calloc(obj_files_buffer_len + 1, sizeof(char));

    for(size_t i = 0; i < obj_files_count; ++i)
    {
        if(!cbrew_file_matches_wildcard(obj_files[i], obj_file_wildcard))
            continue;

        sprintf(obj_files_buffer + strlen(obj_files_buffer), "\"%s\" ", obj_files[i]);
    }

    for(size_t i = 0; i < obj_files_count; ++i)
        free(obj_files[i]);

    free(obj_files);

    obj_files_buffer[obj_files_buffer_len - 1] = '\0';

    return obj_files_buffer;
}

/* Util */

char* cbrew_path(char* path)
{
    CBREW_ASSERT(path != NULL);
    
    for(size_t i = 0; i < strlen(path); ++i)
    {
        if(path[i] == '/')
            path[i] = CBREW_PATH_SEPARATOR;
    }

    return path;
}

char* cbrew_create_flags_str(char** flags, size_t flags_count)
{
    if(flags == NULL || flags_count == 0)
        return calloc(1, sizeof(char));
    
    size_t len = 0;
    for(size_t i = 0; i < flags_count; ++i)
        len += strlen(flags[i]) + strlen(" ");

    char* flags_str = calloc(len + 1, sizeof(char));

    for(size_t i = 0; i < flags_count; ++i)
        sprintf(flags_str + strlen(flags_str), "%s ", flags[i]);

    return flags_str;
}

char* cbrew_create_defines_str(char** defines, size_t defines_count)
{
    if(defines == NULL || defines_count == 0)
        return calloc(1, sizeof(char));
    
    size_t len = 0;
    for(size_t i = 0; i < defines_count; ++i)
        len += strlen("-D") + strlen(defines[i]) + strlen(" ");

    char* defines_str = calloc(len + 1, sizeof(char));

    for(size_t i = 0; i < defines_count; ++i)
        sprintf(defines_str + strlen(defines_str), "-D%s ", defines[i]);

    return defines_str;
}

char* cbrew_create_include_dirs_str(char** include_dirs, size_t include_dirs_count)
{
    if(include_dirs == NULL || include_dirs_count == 0)
        return calloc(1, sizeof(char));
    
    size_t len = 0;
    for(size_t i = 0; i < include_dirs_count; ++i)
        len += strlen("-I") + strlen(include_dirs[i]) + strlen(" ");

    char* include_dirs_str = calloc(len + 1, sizeof(char));

    for(size_t i = 0; i < include_dirs_count; ++i)
        sprintf(include_dirs_str + strlen(include_dirs_str), "-I%s ", include_dirs[i]);

    return include_dirs_str;
}

char* cbrew_create_links_str(char** links, size_t links_count)
{
    if(links == NULL || links_count == 0)
        return calloc(1, sizeof(char));
    
    size_t len = 0;
    for(size_t i = 0; i < links_count; ++i)
        len += strlen("-L. ") + strlen("-l") + strlen(links[i]) + strlen(" ");

    char* links_str = calloc(len + 1, sizeof(char));

    char link_dir[CBREW_FILEPATH_MAX];
    char link_name[CBREW_FILENAME_MAX];
    for(size_t i = 0; i < links_count; ++i)
    {
        const char* last_slash = strrchr(links[i], CBREW_PATH_SEPARATOR);
        if(last_slash == NULL)
        {
            strcpy(link_dir, ".");
            strcpy(link_name, links[i]);
        }
        else
        {
            strcpy(link_dir, links[i]);
            *strrchr(link_dir, CBREW_PATH_SEPARATOR) = '\0';
            strcpy(link_name, last_slash + 1);
        }

        sprintf(links_str + strlen(links_str), "-L%s -l%s ", link_dir, link_name);
    }

    return links_str;
}

CbrewBool cbrew_file_copy(const char* src_path, const char* dest_path)
{
    FILE* src = fopen(src_path, "rb");
    
    if(src == NULL)
        return CBREW_FALSE;

    FILE* dest = fopen(dest_path, "wb");
    
    if(dest == NULL)
    {
        fclose(src);
        return CBREW_FALSE;
    }

    char buffer[4096];
    while(!feof(src))
    {
        size_t bytes = fread(buffer, sizeof(char), sizeof(buffer), src);
        
        if(bytes > 0)
            fwrite(buffer, sizeof(char), bytes, dest);
    }

    fclose(src);
    fclose(dest);

    return CBREW_TRUE;
}

/* IO */

CbrewBool cbrew_command(const char* format, ...)
{
    CBREW_ASSERT(format != NULL);

    char cmd[CBREW_COMMAND_LENGTH_MAX];

    va_list args;
    va_start(args, format);
    vsprintf(cmd, format, args);
    va_end(args);

    CBREW_LOG_CMD(cmd);

    CbrewBool result = system(cmd) == EXIT_SUCCESS;

    return result;
}

CbrewBool cbrew_file_exists(const char* file)
{
    CBREW_ASSERT(file != NULL);
    
    FILE* f = fopen(file, "r");
    
    if(f == NULL)
        return CBREW_FALSE;

    fclose(f);

    return CBREW_TRUE;
}

CbrewBool cbrew_file_delete(const char* file)
{
    CBREW_ASSERT(file != NULL);
    
    return remove(file) == 0;
}

void cbrew_wildcard_directory(char* dir, const char* wildcard)
{
    CBREW_ASSERT(dir != NULL);
    CBREW_ASSERT(wildcard != NULL);

    cbrew_path(strcpy(dir, wildcard));

    for(size_t i = 0; i < strlen(dir); ++i)
    {
        if(dir[i] == '?' || dir[i] == '*' || dir[i] == '[')
        {
            dir[i] = '\0';
            break;
        }
    }

    char* last_path_separator = strrchr(dir, CBREW_PATH_SEPARATOR);

    if(last_path_separator != NULL)
        *last_path_separator = '\0';
    else
        strcpy(dir, ".");
}

CbrewBool cbrew_file_matches_wildcard(const char* filepath, const char* wildcard)
{
    CBREW_ASSERT(filepath != NULL);
    CBREW_ASSERT(wildcard != NULL);

    char filepath_pathed[CBREW_FILEPATH_MAX];
    char wildcard_pathed[CBREW_FILEPATH_MAX];

    cbrew_path(strcpy(filepath_pathed, filepath));
    cbrew_path(strcpy(wildcard_pathed, wildcard));

    const char* fp = filepath_pathed;
    const char* wc = wildcard_pathed;

    if (*wc == '\0' && *fp == '\0')
        return CBREW_TRUE;

    if (*wc == '*' && *(wc + 1) != '\0' && *fp == '\0')
        return CBREW_FALSE;

    if (*wc == '?' || *wc == *fp)
        return cbrew_file_matches_wildcard(fp + 1, wc + 1);

    if (*wc == '[')
    {
        const char *endBracket = strchr(wc, ']');
        if (endBracket == NULL)
            return CBREW_FALSE;
        CbrewBool inRange = CBREW_FALSE;
        for (const char *c = wc+1; c < endBracket; c++)
        {
            if (*c == '-' && c > wc+1 && c < endBracket-1 && isalnum(*(c-1)) && isalnum(*(c+1)))
            {
                if (*(c-1) <= *fp && *fp <= *(c+1))
                    inRange = CBREW_TRUE;
            }
            else if (*c == *fp)
                inRange = CBREW_TRUE;
        }
        if (inRange)
            return cbrew_file_matches_wildcard(endBracket+1, fp+1);
        else
            return CBREW_FALSE;
    }

    if (*wc == '*' && *(wc + 1) == '*')
    {
        wc += 2;

        if(*wc == '\0')
            return CBREW_TRUE;
        
        for(size_t i = 0; i < strlen(fp); ++i)
        {
            if(cbrew_file_matches_wildcard(fp + i, wc))
                return CBREW_TRUE;
        }

        return CBREW_FALSE;
    }

    if (*wc == '*')
    {
        ++wc;

        if(*wc == '\0')
            return strchr(fp, CBREW_PATH_SEPARATOR) == NULL;
        
        for(size_t i = 0; i < strlen(fp); ++i)
        {
            if(cbrew_file_matches_wildcard(fp + i, wc))
                return CBREW_TRUE;
        }
    }

    return CBREW_FALSE;
}

#ifdef CBREW_PLATFORM_WINDOWS

void cbrew_executable_filepath(char* filepath, const char* argv0)
{
    CBREW_ASSERT(filepath != NULL);
    CBREW_ASSERT(argv0 != NULL);

    const char* dot = strrchr(argv0, '.');
    if(dot == NULL)
        sprintf(filepath, "%s.exe", argv0);
    else
        strcpy(filepath, argv0);
}

CbrewBool cbrew_first_file_is_older(const char* first_file, const char* second_file)
{
    CBREW_ASSERT(first_file != NULL);
    CBREW_ASSERT(second_file != NULL);


    char first_path[CBREW_FILEPATH_MAX];
    cbrew_path(strcpy(first_path, first_file));
    WIN32_FIND_DATA first_path_fd;
	HANDLE first_path_find = FindFirstFile(first_path, &first_path_fd);

    if(first_path_find == INVALID_HANDLE_VALUE || first_path_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        FindClose(first_path_find);
        return CBREW_FALSE;
    }

    char second_path[CBREW_FILEPATH_MAX];
    cbrew_path(strcpy(second_path, second_file));
    WIN32_FIND_DATA second_path_fd;
    HANDLE second_path_find = FindFirstFile(second_path, &second_path_fd);

    if(second_path_find == INVALID_HANDLE_VALUE || second_path_fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        FindClose(first_path_find);
        FindClose(second_path_find);

        return CBREW_FALSE;
    }

    CbrewBool result = CompareFileTime(&second_path_fd.ftLastWriteTime, &first_path_fd.ftLastWriteTime) == 1;

    FindClose(first_path_find);
    FindClose(second_path_find);

    return result;
}

CbrewBool cbrew_file_rename(const char* old_name, const char* new_name)
{
    CBREW_ASSERT(old_name != NULL);
    CBREW_ASSERT(new_name != NULL);
    
    char old_name_path[CBREW_FILEPATH_MAX];
    cbrew_path(strcpy(old_name_path, old_name));

    char new_name_path[CBREW_FILEPATH_MAX];
    cbrew_path(strcpy(new_name_path, new_name));

    CbrewBool result = MoveFileEx(old_name_path, new_name_path, MOVEFILE_REPLACE_EXISTING) != 0;

    return result;
}

char** cbrew_find_files(const char* dir, size_t* files_count)
{
    CBREW_ASSERT(dir != NULL);

    char** files = NULL;
	
	size_t files_found = 0;

    char dir_search_path[CBREW_FILEPATH_MAX];
    strcpy(dir_search_path, dir);
    cbrew_path(dir_search_path);
    strcat(dir_search_path, "\\*");

	WIN32_FIND_DATA fd;
	HANDLE find = FindFirstFile(dir_search_path, &fd);

	if (find != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
			{
				++files_found;
				files = realloc(files, files_found * sizeof(char*));

				files[files_found - 1] = malloc((strlen(dir) + 1 + strlen(fd.cFileName) + 1) * sizeof(char));
                sprintf(files[files_found - 1], "%s%c%s", dir, CBREW_PATH_SEPARATOR, fd.cFileName);
			}
		} while (FindNextFile(find, &fd));

		FindClose(find);
	}

	if (files_count != NULL)
		*files_count = files_found;

	return files;
}

char** cbrew_find_files_recursive(const char* dir, size_t* files_count)
{
    CBREW_ASSERT(dir != NULL);
    
    char** files = NULL;
	
	size_t files_found = 0;

    char dir_search_path[CBREW_FILEPATH_MAX];
    strcpy(dir_search_path, dir);
    cbrew_path(dir_search_path);
    strcat(dir_search_path, "\\*");

	WIN32_FIND_DATA fd;
	HANDLE find = FindFirstFile(dir_search_path, &fd);

	if (find != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				++files_found;
				files = realloc(files, files_found * sizeof(char*));

				files[files_found - 1] = malloc((strlen(dir) + 1 + strlen(fd.cFileName) + 1) * sizeof(char));
                sprintf(files[files_found - 1], "%s%c%s", dir, CBREW_PATH_SEPARATOR, fd.cFileName);
			}
            else if(strcmp(fd.cFileName, ".") != 0 && strcmp(fd.cFileName, "..") != 0)
            {
                char sub_dir[CBREW_FILEPATH_MAX];
                sprintf(sub_dir, "%s%c%s", dir, CBREW_PATH_SEPARATOR, fd.cFileName);

                size_t sub_files_count = 0;
                char** sub_files = cbrew_find_files_recursive(sub_dir, &sub_files_count);

                files = realloc(files, (files_found + sub_files_count) * sizeof(char*));

                for(size_t i = 0; i < sub_files_count; ++i)
                    files[files_found + i] = sub_files[i];

				files_found += sub_files_count;

                free(sub_files);
            }
		} while (FindNextFile(find, &fd));

		FindClose(find);
	}

	if (files_count != NULL)
		*files_count = files_found;

	return files;
}

CbrewBool cbrew_dir_exists(const char* dir)
{
    CBREW_ASSERT(dir != NULL);

    DWORD file_attributes = GetFileAttributes(dir);

    return file_attributes != INVALID_FILE_ATTRIBUTES && (file_attributes & FILE_ATTRIBUTE_DIRECTORY);
}

CbrewBool cbrew_dir_create(const char* dir)
{
    CBREW_ASSERT(dir != NULL);
    
    for(size_t i = 0; i < strlen(dir); ++i)
    {
        if(dir[i] != CBREW_PATH_SEPARATOR)
            continue;
        
        char dir_path[CBREW_FILEPATH_MAX];
        strncpy(dir_path, dir, i);
        dir_path[i] = '\0';

        if(CreateDirectory(dir_path, NULL) == 0 && GetLastError() != ERROR_ALREADY_EXISTS)
            return CBREW_FALSE;
    }

    if(CreateDirectory(dir, NULL) != 0)
        return CBREW_TRUE;

    return GetLastError() != ERROR_ALREADY_EXISTS;
}

CbrewBool cbrew_self_destruct(void)
{
    static const char* bat_script_template = 
        ":Repeat\r\n"
        "del \"%s\"\r\n"
        "if exist \"%s\" goto Repeat\r\n"
       "del \"%s\"";

    char executable_filepath[CBREW_FILEPATH_MAX];
    char bat_filepath[CBREW_FILEPATH_MAX];

    GetTempPath(CBREW_FILEPATH_MAX, bat_filepath);
    strcat(bat_filepath, "cbrew.bat");

    if(!GetCurrentDirectory(CBREW_FILEPATH_MAX, executable_filepath))
        return CBREW_FALSE;

    strcat(executable_filepath, CBREW_PATH_SEPARATOR_STR CBREW_OLD_NAME);

    printf("Filepath: %s\n", executable_filepath);

    const HANDLE bat_handle = CreateFile(bat_filepath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if(bat_handle != INVALID_HANDLE_VALUE)
    {
        char* bat_script = (char*)alloca(strlen(bat_script_template) + strlen(executable_filepath) * 2 + strlen(bat_filepath) + 20);
    
        wsprintf(bat_script, bat_script_template, executable_filepath, executable_filepath, bat_filepath);

        DWORD len;
        WriteFile(bat_handle, bat_script, strlen(bat_script), &len, NULL);
        CloseHandle(bat_handle);

        ShellExecute(NULL, "open", bat_filepath, NULL, NULL, SW_HIDE);
    }

    return CBREW_TRUE;
}

#elif defined(CBREW_PLATFORM_LINUX)

void cbrew_executable_filepath(char* filepath, const char* argv0)
{
    CBREW_ASSERT(filepath != NULL);
    CBREW_ASSERT(argv0 != NULL);

    strcpy(filepath, argv0);
}

CbrewBool cbrew_first_file_is_older(const char* first_file, const char* second_file)
{
    CBREW_ASSERT(first_file != NULL);
    CBREW_ASSERT(second_file != NULL);

    struct stat first_stat;
    struct stat second_stat;

    if (stat(first_file, &first_stat) != 0 || S_ISDIR(first_stat.st_mode))
        return CBREW_FALSE;

    if (stat(second_file, &second_stat) != 0 || S_ISDIR(second_stat.st_mode))
        return CBREW_FALSE;

    return second_stat.st_mtime > first_stat.st_mtime;
}

CbrewBool cbrew_file_rename(const char* old_name, const char* new_name)
{
    CBREW_ASSERT(old_name != NULL);
    CBREW_ASSERT(new_name != NULL);

    return rename(old_name, new_name) == 0;
}

char** cbrew_find_files(const char* dir, size_t* files_count)
{
    CBREW_ASSERT(dir != NULL);

    char** files = NULL;
    size_t files_found = 0;

    DIR* d = opendir(dir);
    if (d)
    {
        struct dirent* de;
        while ((de = readdir(d)) != NULL)
        {
            if (de->d_type != DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
            {
                ++files_found;
                files = realloc(files, files_found * sizeof(char*));

                files[files_found - 1] = malloc((strlen(dir) + 1 + strlen(de->d_name) + 1) * sizeof(char));
                sprintf(files[files_found - 1], "%s/%s", dir, de->d_name);
            }
        }

        closedir(d);
    }

    if (files_count != NULL)
        *files_count = files_found;

    return files;
}

char** cbrew_find_files_recursive(const char* dir, size_t* files_count)
{
    CBREW_ASSERT(dir != NULL);

    char** files = NULL;
    size_t files_found = 0;

    DIR* d = opendir(dir);
    if (d)
    {
        struct dirent* de;
        while ((de = readdir(d)) != NULL)
        {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
            {
                char path[CBREW_FILEPATH_MAX];
                sprintf(path, "%s/%s", dir, de->d_name);

                struct stat sb;
                if (stat(path, &sb) == 0)
                {
                    if (S_ISDIR(sb.st_mode))
                    {
                        size_t sub_files_count = 0;
                        char** sub_files = cbrew_find_files_recursive(path, &sub_files_count);

                        files = realloc(files, (files_found + sub_files_count) * sizeof(char*));

                        for(size_t i = 0; i < sub_files_count; ++i)
                            files[files_found + i] = sub_files[i];

                        files_found += sub_files_count;

                        free(sub_files);
                    }
                    else
                    {
                        ++files_found;
                        files = realloc(files, files_found * sizeof(char*));

                        files[files_found - 1] = malloc((strlen(path) + 1) * sizeof(char));
                        strcpy(files[files_found - 1], path);
                    }
                }
            }
        }

        closedir(d);
    }

    if (files_count != NULL)
        *files_count = files_found;

    return files;
}

CbrewBool cbrew_dir_exists(const char* dir)
{
    CBREW_ASSERT(dir != NULL);

    struct stat sb;

    return stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode);
}

CbrewBool cbrew_dir_create(const char* dir)
{
    CBREW_ASSERT(dir != NULL);
    
    char dir_path[CBREW_FILEPATH_MAX];
    for(size_t i = 0; i < strlen(dir); ++i)
    {
        if(dir[i] != CBREW_PATH_SEPARATOR)
            continue;
        
        strncpy(dir_path, dir, i);
        dir_path[i] = '\0';

        if(mkdir(dir_path, 0755) != 0 && errno != EEXIST)
            return CBREW_FALSE;
    }

    if(mkdir(dir, 0755) == 0)
        return CBREW_TRUE;

    return errno == EEXIST;
}

CbrewBool cbrew_self_destruct(void)
{
    return unlink(CBREW_OLD_NAME) == 0;
}

#else
#error "CBREW is currently not implemented for this platform!"
#endif

#endif

#endif
