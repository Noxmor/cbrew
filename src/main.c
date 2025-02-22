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

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    if (cbrew_first_file_is_older(".cbrew/cbrew", "cbrew.c"))
    {
        rebuild();
    }

    return cbrew_command(".cbrew/cbrew") == 0;
}
