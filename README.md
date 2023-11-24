# cbrew
Everything anyone should ever need to compile a C project should be a C compiler.

***

## Getting Started
1. Start by cloning this repository with `git clone https://github.com/Noxmor/cbrew` into the common root directory of your project(s).
2. Create a file named `cbrew.c` inside the `cbrew` folder.
3. Open `cbrew.c` and start coding and customizing your build tool.
4. Compile `cbrew.c` into an executable inside your root directory.
5. Done! If you used the `CBREW_AUTO_REBUILD` macro, cbrew will automatically recompile itself whenever you modify the source code of `cbrew.c`.

***

## How cbrew works
With cbrew you can create projects and configurations that can than be automatically compiled by cbrew. Every `project` has a unique name and a type. Every project needs to have at least one `configuration` that tells cbrew how to compile the project. Every `configuration` for a certain project has a unique name and more importantly a `target directory` and an `object directory`. The final product (depending on the type of the project) will be placed inside the `target directory`, while all object files will be placed inside the `object directory`. This is useful for reducing compile time for larger projects, because cbrew will only recompile source files that have no or an outdated object file. If the object file is still up to date, the source file won't be compiled again.

### Bootstrapping
Normally, you would need to recompile the cbrew executable every time you make changes to the `cbrew.c` source file. This is tedious work that we want to avoid. Therefore, you should use the `CBREW_AUTO_REBUILD` macro to enable automatic recompilation of the cbrew executable. When using this, you only need to compile cbrew by hand once (bootstrapping). When using this macro, cbrew will check every time it is run if its source code was modified and if so, recompile itself, before starting the actual execution of commands to compile all your projects.

### Compiler detection
cbrew will detect your C compiler that you used to bootstrap cbrew. cbrew will henceforward use exactly this compiler when executing commands.

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

## Compiling a simple "Hello World!" program
Inside `cbrew.c`:
```c
#define CBREW_IMPLEMENTATION
#include "cbrew.h"

int main(int argc, char** argv)
{
  CbrewProject* project = CBREW_PRJ_NEW("Hello-World", CBREW_PROJECT_TYPE_APP);

  CBREW_PRJ_FILES(project, "./hello_world.c");

  CbrewConfig* debug_config = CBREW_CFG_NEW(project, "Debug", "bin/Hello-World-Debug", "bin-int/Hello-World-Debug");

  cbrew_build();

  return 0;
}
```

***

## Compiling a static library
Inside `cbrew.c`:
```c
#define CBREW_IMPLEMENTATION
#include "cbrew.h"

int main(int argc, char** argv)
{
  CbrewProject* static_lib = CBREW_PRJ_NEW("Static-Libary-Name", CBREW_PROJECT_TYPE_STATIC_LIB);

  CBREW_PRJ_FILES(project, "./my_lib/**.c");

  CbrewConfig* debug_config = CBREW_CFG_NEW(static_lib, "Debug", "bin/Static-Libary-Name-Debug", "bin-int/Static-Libary-Name-Debug");

  cbrew_build();

  return 0;
}
```

***

## Compiling a dynamic library
Inside `cbrew.c`:
```c
#define CBREW_IMPLEMENTATION
#include "cbrew.h"

int main(int argc, char** argv)
{
  CbrewProject* dynamic_lib = CBREW_PRJ_NEW("Dynamic-Libary-Name", CBREW_PROJECT_TYPE_DYNAMIC_LIB);

  CBREW_PRJ_FILES(project, "./my_lib/**.c");

  CbrewConfig* debug_config = CBREW_CFG_NEW(dynamic_lib, "Debug", "bin/Dynamic-Libary-Name-Debug", "bin-int/Dynamic-Libary-Name-Debug");

  cbrew_build();

  return 0;
}
```

***

## Compiling multiple projects
Inside `cbrew.c`:
```c
#define CBREW_IMPLEMENTATION
#include "cbrew.h"

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

int main(int argc, char** argv)
{
  CBREW_AUTO_REBUILD(argc, argv);

  create_foo_project();
  create_bar_project();

  cbrew_build();

  return 0;
}
```
