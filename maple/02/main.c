#include <stdio.h>
#include <dlfcn.h>

int main() {
    // carrega a biblioteca compartilhada
    void* handle = dlopen("/opt/maple2021/bin.X86_64_LINUX/libmaple.so", RTLD_NOW);
    if (!handle) {
        printf("Erro ao carregar a biblioteca compartilhada: %s\n", dlerror());
        return 1;
    }

    // obtem o ponteiro para a função "sin" da biblioteca
    double (*sin)(double) = dlsym(handle, "_Z3sind");

    // verifica se houve algum erro ao obter a função
    char* error = dlerror();
    if (error != NULL) {
        printf("Erro ao obter a função: %s\n", error);
        dlclose(handle);
        return 1;
    }

    // chama a função "sin"
    double angulo = 1.57; // 90 graus em radianos
    double resultado = sin(angulo);
    printf("O seno de %f é %f\n", angulo, resultado);

    // fecha a biblioteca compartilhada
    dlclose(handle);

    return 0;
}
