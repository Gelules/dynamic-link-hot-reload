#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

typedef void (*fun_test_hello_world)(void);

int reload_hello_world(const char *lib_path, const char *symbol, void **lib_test, fun_test_hello_world *fun_hello_world)
{
    dlclose(*lib_test);
    *lib_test = dlopen(lib_path, RTLD_NOW);
    if (*lib_test == NULL)
    {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }

    *fun_hello_world = (void (*)(void))(intptr_t) dlsym(*lib_test, symbol);
    if (*fun_hello_world == NULL)
    {
        fprintf(stderr, "Cannot load %s symbol\n", symbol);
        return -1;
    }

    return 0;
}

int main(void)
{
    int reload = 0;
    char command[4096] = { 0 };
    size_t command_len = 0;
    const char *lib_path = "./libtest.so";
    const char *symbol = "hello_world";
    fun_test_hello_world fun_hello_world = NULL;
    void *lib_test = dlopen(lib_path, RTLD_NOW);

    if (lib_test == NULL)
    {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    fun_hello_world = (void (*)(void))(intptr_t) dlsym(lib_test, symbol);
    if (fun_hello_world == NULL)
    {
        fprintf(stderr, "Cannot load %s symbol\n", symbol);
        return 1;
    }

    while (1)
    {
        printf("$ ");
        fgets(command, sizeof (command), stdin);
        command_len = strlen(command);
        if (command_len > 0 && command[command_len - 1] == '\n')
            command[command_len - 1] = '\0';

        if (!strcmp(command, "print"))
        {
            fun_hello_world();
        }
        else if (!strcmp(command, "reload"))
        {
            reload = reload_hello_world(lib_path, symbol, &lib_test, &fun_hello_world);
            if (reload != 0)
            {
                fprintf(stderr, "error; %d - %s\n", reload, dlerror());
                return 2;
            }
        }
        else if (!strcmp(command, "exit"))
        {
            break;
        }
        else
        {
            fprintf(stderr, "Command not implemented\n");
        }
    }

    dlclose(lib_test);
    return 0;
}
