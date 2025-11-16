#define CBREW_IMPLEMENTATION
#include "cbrew.h"

static void help_callback(void);

static void version_callback(void);

static void init(int argc, char** argv);

#define CLAPX_FLAGS \
    CLAPX_BOOL_FLAG(help, "help", 'h', "Show this help message and exit.", help_callback) \
    CLAPX_BOOL_FLAG(version, "version", 'v', "Show version information and exit.", version_callback)

#define CLAPX_SUBCOMMANDS \
    CLAPX_SUBCOMMAND(init, "init", "Initializes cbrew.", init)

#define CLAPX_IMPLEMENTATION
#include <clapx/clapx.h>

static void help_callback(void)
{
    clapx_print_usage_str("cbrew");
    exit(EXIT_SUCCESS);
}

static void version_callback(void)
{
    printf("cbrew version %s\n", CBREW_VERSION_STRING);
    exit(EXIT_SUCCESS);
}

static void rebuild(void)
{
    if (!cbrew_dir_exists(".cbrew"))
    {
        if (!cbrew_dir_create(".cbrew"))
        {
            exit(EXIT_FAILURE);
        }
    }

    const char* compiler = getenv("CBREW_CC");
    if (compiler == NULL)
    {
        compiler = "gcc";
    }

    if (!cbrew_command("%s -Wall -Wextra -O3 cbrew.c -o.cbrew/cbrew", compiler))
    {
        exit(EXIT_FAILURE);
    }
}

static void init(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    if (cbrew_file_exists("cbrew.c"))
    {
        CBREW_LOG_ERROR("cbrew has already been initialized!");

        return;
    }

    printf("Initializing cbrew...\n");

    static const char* cbrew_preset =
        "#define CBREW_IMPLEMENTATION\n"
        "#include <cbrew/cbrew.h>\n\n"
        "int main(void)\n"
        "{\n"
        "    CbrewProject* project = CBREW_PRJ_NEW(\"Hello-World\", CBREW_PROJECT_TYPE_APP);\n\n"
        "    CBREW_PRJ_FILES(project, \"./hello_world.c\");\n\n"
        "    CbrewConfig* debug_config = CBREW_CFG_NEW(project, \"Debug\", \"bin/Hello-World-Debug\", \"bin-int/Hello-World-Debug\");\n\n"
        "    cbrew_build();\n"
        "}";

    FILE* f = fopen("cbrew.c", "w");
    if (f == NULL)
    {
        return;
    }

    if (fwrite(cbrew_preset, sizeof(char), strlen(cbrew_preset), f) != strlen(cbrew_preset)
        || fclose(f))
    {
        perror("Error initializing cbrew");
    }
    else
    {
        printf("Successfully initialized cbrew\n");
    }
}

int main(int argc, char** argv)
{
    if (clapx_parse_args(argc, argv) != CLAPX_SUCCESS)
    {
        clapx_print_usage_str(argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 1)
    {
        if (!cbrew_file_exists(".cbrew/cbrew") || cbrew_first_file_is_older(".cbrew/cbrew", "cbrew.c"))
        {
            rebuild();
        }

        return cbrew_command(".cbrew" CBREW_PATH_SEPARATOR_STR "cbrew") == 0;
    }

    return EXIT_SUCCESS;
}
