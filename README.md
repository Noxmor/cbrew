# cbrew
Everything anyone should ever need to compile a C project should be a C compiler.

***

## About
`cbrew` comes in two components:
1. The `cbrew` executable
2. The `cbrew.h` header

***

## Installing

### Linux
1. Start by cloning this repository with the following command:
```bash
git clone https://github.com/Noxmor/cbrew
```
2. Run the `install.sh` script.
3. Done!

***

### Windows
1. Start by cloning this repository with the following command:
```bash
git clone https://github.com/Noxmor/cbrew
```
2. Run the `install.bat` script.
3. Update your `PATH` environment variable to include `C:\cbrew\bin`.
4. Update your compiler's include path environment variable (e.g. `C_INCLUDE_PATH` for `gcc`) to include `C:\cbrew\include`.
5. Done.

***

## Setup cbrew for an existing project
1. Navigate into the root directory of your project 
2. Create a file named `cbrew.c`.
3. Open `cbrew.c` and start coding and customizing. See [Examples](#examples) for more information.
4. Done! You can now compile your project(s) by just running `cbrew` inside the directory with the `cbrew.c` file.

***

## Configuration for .gitignore
If you use `cbrew` in a git project, you should add the following to your `.gitignore`:
```bash
.cbrew/
```
> [!NOTE]
Ideally, you would want to ignore your `target directory` and `object directory` as well. However, these are defined inside your `cbrew.c` file, so it's up to you to add the correct paths in your `.gitignore`.

***

## How cbrew works
With `cbrew` you can create projects and configurations that can than be automatically (re-)compiled. Every `project` has a unique name and a type. Every project needs to have at least one `configuration` that tells cbrew how to compile the project. Every `configuration` for a certain project has a unique name and more importantly a `target directory` and an `object directory`. The final product (depending on the type of the project) will be placed inside the `target directory`, while all object files will be placed inside the `object directory`. The `cbrew` executable automatically generates a **local** `cbrew` executable based on the `cbrew.c` file, which will be in charge of actually compiling your project(s). The **local** executable can be found at `./.cbrew/cbrew`, relative to the location of the `cbrew.c` file.

### Bootstrapping
Normally, you would need to recompile your **local** `cbrew` executable every time you make changes to the `cbrew.c` source file. This is tedious work that we want to avoid. Therefore, when you run `cbrew` it will automatically check if your **local** executable is outdated and if so, recompile it before executing it. If `cbrew` can't find an existing executable, it will automatically compile your `cbrew.c` file for the first time before executing it, thus bootstrapping it.

### Compiler detection
`cbrew` will detect your C compiler based on the `CBREW_CC` environment variable. If this variable is not set, `cbrew` will fallback to use `gcc`.

### How to specify paths correctly
Whenever you're specifying a filepath e.g. `foo/bar/buzz` **always** use `/` as the path separator character. cbrew will automatically change the `/` character if your platform uses a different path separator.

### Asserts
Many internal functions of cbrew are filled with asserts to detect data that would result in a crash or undefined behaviour. If you're encountering weird behaviour of cbrew, try to enable asserts first and check if cbrew now raises any asserts. You can enable asserts by defining the `CBREW_ENABLE_ASSERTS` macro before including `cbrew.h`.

### Console colors
By default, cbrew logs everything in your standard console's text color. You can enable console colors by defining the `CBREW_ENABLE_CONSOLE_COLORS` macro before including `cbrew.h`. cbrew comes with a default set of colors, but if you don't like them, you can change them.

The logging colors that cbrew uses internally are:
* `CBREW_CONSOLE_COLOR_ERROR`
* `CBREW_CONSOLE_COLOR_WARN`
* `CBREW_CONSOLE_COLOR_INFO`
* `CBREW_CONSOLE_COLOR_TRACE`
* `CBREW_CONSOLE_COLOR_COMMAND`
* `CBREW_CONSOLE_COLOR_PROJECT`
* `CBREW_CONSOLE_COLOR_CONFIG`
* `CBREW_CONSOLE_COLOR_TIME`

If you want to change any of these colors, define the macro with any of cbrew's color macros before including `cbrew.h`.
The possible console colors are:
* `CBREW_CONSOLE_COLOR_BLACK`
* `CBREW_CONSOLE_COLOR_DARK_BLUE`
* `CBREW_CONSOLE_COLOR_DARK_GREEN`
* `CBREW_CONSOLE_COLOR_DARK_AQUA`
* `CBREW_CONSOLE_COLOR_DARK_RED`
* `CBREW_CONSOLE_COLOR_DARK_PURPLE`
* `CBREW_CONSOLE_COLOR_GOLD`
* `CBREW_CONSOLE_COLOR_GRAY`
* `CBREW_CONSOLE_COLOR_DARK_GRAY`
* `CBREW_CONSOLE_COLOR_BLUE`
* `CBREW_CONSOLE_COLOR_GREEN`
* `CBREW_CONSOLE_COLOR_AQUA`
* `CBREW_CONSOLE_COLOR_RED`
* `CBREW_CONSOLE_COLOR_LIGHT_PURPLE`
* `CBREW_CONSOLE_COLOR_YELLOW`
* `CBREW_CONSOLE_COLOR_WHITE`

**Example**: Changing the default red color of error logs to white would look like this:
```c
#define CBREW_CONSOLE_COLOR_ERROR CBREW_CONSOLE_COLOR_WHITE
```

### Commands under the hood
Under the hood cbrew performs a variety of commands, so you don't have to manually type them in the console every time. By default, commands are not logged. If you want to have visual feedback on what cbrew does under the hood, you can define the `CBREW_LOG_COMMANDS` macro before including `cbrew.h`.

***

## Examples

### Compiling a simple "Hello World!" program
Inside `cbrew.c`:
```c
#define CBREW_IMPLEMENTATION
#include <cbrew/cbrew.h>

int main(void)
{
    CbrewProject* project = CBREW_PRJ_NEW("Hello-World", CBREW_PROJECT_TYPE_APP);

    CBREW_PRJ_FILES(project, "./hello_world.c");

    CbrewConfig* debug_config = CBREW_CFG_NEW(project, "Debug", "bin/Hello-World-Debug", "bin-int/Hello-World-Debug");

    cbrew_build();

    return 0;
}
```

***

### Compiling a static library
Inside `cbrew.c`:
```c
#define CBREW_IMPLEMENTATION
#include <cbrew/cbrew.h>

int main(void)
{
    CbrewProject* static_lib = CBREW_PRJ_NEW("Static-Libary-Name", CBREW_PROJECT_TYPE_STATIC_LIB);

    CBREW_PRJ_FILES(static_lib, "./my_lib/**.c");

    CbrewConfig* debug_config = CBREW_CFG_NEW(static_lib, "Debug", "bin/Static-Libary-Name-Debug", "bin-int/Static-Libary-Name-Debug");

    cbrew_build();

    return 0;
}
```

***

### Compiling a dynamic library
Inside `cbrew.c`:
```c
#define CBREW_IMPLEMENTATION
#include <cbrew/cbrew.h>

int main(void)
{
    CbrewProject* dynamic_lib = CBREW_PRJ_NEW("Dynamic-Libary-Name", CBREW_PROJECT_TYPE_DYNAMIC_LIB);

    CBREW_PRJ_FILES(dynamic_lib, "./my_lib/**.c");

    CbrewConfig* debug_config = CBREW_CFG_NEW(dynamic_lib, "Debug", "bin/Dynamic-Libary-Name-Debug", "bin-int/Dynamic-Libary-Name-Debug");

    cbrew_build();

    return 0;
}
```

***

### Compiling multiple projects
Inside `cbrew.c`:
```c
#define CBREW_IMPLEMENTATION
#include <cbrew/cbrew.h>

void create_foo_project(void)
{
    CbrewProject* project = CBREW_PRJ_NEW("Foo", CBREW_PROJECT_TYPE_STATIC_LIB);
    CBREW_PRJ_FILES(project, "./foo/**.c");
    CBREW_PRJ_INCLUDE_DIR(project, "foo");
    CBREW_PRJ_FLAG(project, "-Wall");
    CBREW_PRJ_FLAG(project, "-Wextra");

    CbrewConfig* debug_config = CBREW_CFG_NEW(project, "Debug", "bin/Foo-Debug", "bin-int/Foo-Debug");
    CBREW_CFG_DEFINE(debug_config, "FOO_DEBUG");

    CbrewConfig* release_config = CBREW_CFG_NEW(project, "Release", "bin/Foo-Release", "bin-int/Foo-Release");
    CBREW_CFG_DEFINE(release_config, "FOO_RELEASE");
    CBREW_CFG_FLAG(release_config, "-O3");
}

void create_bar_project(void)
{
    CbrewProject* project = CBREW_PRJ_NEW("Bar", CBREW_PROJECT_TYPE_APP);
    CBREW_PRJ_FILES(project, "./bar/**.c");
    CBREW_PRJ_INCLUDE_DIR(project, "bar");
    CBREW_PRJ_FLAG(project, "-Wall");
    CBREW_PRJ_FLAG(project, "-Wextra");
    CBREW_PRJ_LINK(project, "Foo.lib");

    CbrewConfig* debug_config = CBREW_CFG_NEW(project, "Debug", "bin/Bar-Debug", "bin-int/Bar-Debug");
    CBREW_CFG_DEFINE(debug_config, "BAR_DEBUG");

    CbrewConfig* release_config = CBREW_CFG_NEW(project, "Release", "bin/Bar-Release", "bin-int/Bar-Release");
    CBREW_CFG_DEFINE(release_config, "BAR_RELEASE");
    CBREW_CFG_FLAG(release_config, "-O3");
}

int main(void)
{
    create_foo_project();
    create_bar_project();

    cbrew_build();

    return 0;
}
```
