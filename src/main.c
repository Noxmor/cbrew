#define CBREW_IMPLEMENTATION
#include "cbrew.h"

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

static void init(void)
{
    if (cbrew_file_exists("cbrew.c"))
    {
        CBREW_LOG_ERROR("cbrew has already been initialized!");

        return;
    }

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

    fwrite(cbrew_preset, sizeof(char), strlen(cbrew_preset), f);
    fclose(f);
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        if (!cbrew_file_exists(".cbrew/cbrew") || cbrew_first_file_is_older(".cbrew/cbrew", "cbrew.c"))
        {
            rebuild();
        }

        return cbrew_command(".cbrew" CBREW_PATH_SEPARATOR_STR "cbrew") == 0;
    }

    if (strcmp(argv[1], "init") == 0)
    {
        init();
    }
    else
    {
        CBREW_LOG_ERROR("Invalid subcommand!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
