#include <dlfcn.h>
#include <stdio.h>

int main() {
  // carrega a biblioteca compartilhada
  void *handle =
      dlopen("/opt/maple2021/bin.X86_64_LINUX/libmaple.so", RTLD_NOW);
  if (!handle) {
    printf("Erro ao carregar a biblioteca compartilhada: %s\n", dlerror());
    return 1;
  }

  // obtem o ponteiro para a função "sqrt" da biblioteca
  // double (*sqrt) (double) = dlsym (handle, "_Z4sqrtd");
  // correção:
  double (*sqrt)(double) = dlsym(handle, "hfu_sqrt");

  // verifica se houve algum erro ao obter a função
  char *error = dlerror();
  if (error != NULL) {
    printf("Erro ao obter a função: %s\n", error);
    dlclose(handle);
    return 1;
  }

  // chama a função "sqrt"
  double numero = 16.0;
  double resultado = sqrt(numero);
  printf("A raiz quadrada de %f é %f\n", numero, resultado);

  // fecha a biblioteca compartilhada
  dlclose(handle);

  return 0;
}
