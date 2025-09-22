#include <cstdlib>
#include <dlfcn.h>
#include <iostream>
#include <string>
#include <vector>

class MapleLibraryExplorer
{
private:
    void* handle;
    std::string library_path;

public:
    MapleLibraryExplorer (const std::string& path)
        : handle (nullptr), library_path (path) {}

    bool loadLibrary()
    {
        // Configura LD_LIBRARY_PATH antes de carregar
        std::string maple_dir = "/opt/maple2021/bin.X86_64_LINUX";
        std::string ld_path = "LD_LIBRARY_PATH=" + maple_dir;
        putenv (const_cast<char*> (ld_path.c_str()));

        std::cout << "Tentando carregar: " << library_path << std::endl;

        // Carrega a biblioteca com RTLD_LAZY para evitar resolver símbolos
        // imediatamente
        handle = dlopen (library_path.c_str(), RTLD_LAZY | RTLD_LOCAL);

        if (!handle)
        {
            std::cerr << "Erro ao carregar biblioteca: " << dlerror() << std::endl;
            return false;
        }

        std::cout << "Biblioteca carregada com sucesso!" << std::endl;
        return true;
    }

    // Tenta encontrar funções com nomes comuns de APIs Maple
    void probeFunctions()
    {
        std::cout << "\n=== TESTANDO FUNÇÕES COMUNS ===" << std::endl;

        std::vector<std::string> common_functions =
        {
            // Possíveis funções de inicialização
            "MapleInit", "maple_init", "InitMaple", "StartMaple", "maple_start",

            // Possíveis funções de avaliação
            "MapleEval", "maple_eval", "EvalMapleStatement", "maple_execute",
            "ToMaple", "FromMaple",

            // Possíveis funções de cleanup
            "MapleCleanup", "maple_cleanup", "StopMaple", "maple_stop",

            // Funções típicas de bibliotecas matemáticas
            "NewKernel", "StopKernel", "StartKernel", "EvalMapleProc", "MapleProc",

            // Baseado no que sabemos sobre Maple
            "maplec_init", "maplec_start", "maplec_eval", "mserver_init"
        };

        for (const auto& func_name : common_functions)
        {
            void* func_ptr = dlsym (handle, func_name.c_str());
            if (func_ptr)
            {
                std::cout << "✓ Função encontrada: " << func_name << " @ " << func_ptr
                          << std::endl;
            }
        }
    }

    // Tenta encontrar variáveis/constantes exportadas
    void probeVariables()
    {
        std::cout << "\n=== TESTANDO VARIÁVEIS COMUNS ===" << std::endl;

        std::vector<std::string> common_variables =
        {
            "maple_version", "MAPLE_VERSION", "MapleVersion", "kernel_version",
            "lib_version"
        };

        for (const auto& var_name : common_variables)
        {
            void* var_ptr = dlsym (handle, var_name.c_str());
            if (var_ptr)
            {
                std::cout << "✓ Variável encontrada: " << var_name << " @ " << var_ptr
                          << std::endl;

                // Tenta interpretar como string ou int
                try
                {
                    char* str_val = *static_cast<char**> (var_ptr);
                    if (str_val)
                    {
                        std::cout << "    Valor (como string): " << str_val << std::endl;
                    }
                }
                catch (...)
                {
                    try
                    {
                        int int_val = *static_cast<int*> (var_ptr);
                        std::cout << "    Valor (como int): " << int_val << std::endl;
                    }
                    catch (...)
                    {
                        std::cout << "    Não foi possível interpretar o valor"
                                  << std::endl;
                    }
                }
            }
        }
    }

    // Analisa símbolos através de dladdr (se disponível)
    void analyzeSymbols()
    {
        std::cout << "\n=== ANÁLISE DE SÍMBOLOS ===" << std::endl;

        // Tenta obter informações sobre o próprio handle
        Dl_info info;
        if (dladdr (handle, &info))
        {
            std::cout << "Arquivo: " << info.dli_fname << std::endl;
            if (info.dli_sname)
            {
                std::cout << "Símbolo base: " << info.dli_sname << std::endl;
            }
        }

        // Lista algumas funções C++ típicas (se existirem)
        std::cout << "\nProcurando por construtores/destrutores C++:" << std::endl;

        std::vector<std::string> cpp_symbols =
        {
            "_init",
            "_fini",
            "__cxa_finalize",
            "_ZN", // Início de símbolos C++ mangled
        };

        for (const auto& symbol : cpp_symbols)
        {
            void* sym_ptr = dlsym (handle, symbol.c_str());
            if (sym_ptr)
            {
                std::cout << "✓ Símbolo C++: " << symbol << std::endl;
            }
        }
    }

    // Método para testar se conseguimos executar algo básico
    void testBasicExecution()
    {
        std::cout << "\n=== TESTE DE EXECUÇÃO BÁSICA ===" << std::endl;

        // Procura por uma função que pareça ser de inicialização
        typedef int (*init_func_t)();
        typedef int (*init_func_args_t) (int, char**);

        std::vector<std::string> init_candidates = {"MapleInit", "maple_init",
                                                    "StartMaple", "NewKernel"
                                                   };

        for (const auto& func_name : init_candidates)
        {
            void* func_ptr = dlsym (handle, func_name.c_str());
            if (func_ptr)
            {
                std::cout << "Tentando executar " << func_name << "..." << std::endl;
                try
                {
                    init_func_t init_func = reinterpret_cast<init_func_t> (func_ptr);
                    int result = init_func();
                    std::cout << "✓ " << func_name << " executado, retorno: " << result
                              << std::endl;
                }
                catch (const std::exception& e)
                {
                    std::cout << "✗ Erro ao executar " << func_name << ": " << e.what()
                              << std::endl;
                }
                catch (...)
                {
                    std::cout << "✗ Erro desconhecido ao executar " << func_name
                              << std::endl;
                }
                break; // Para no primeiro que conseguir executar
            }
        }
    }

    // Método para extrair informações sobre a biblioteca
    void printLibraryInfo()
    {
        std::cout << "\n=== INFORMAÇÕES DA BIBLIOTECA ===" << std::endl;
        std::cout << "Caminho: " << library_path << std::endl;
        std::cout << "Handle: " << handle << std::endl;

        // Informações do sistema sobre a biblioteca
        std::cout << "\nExecutando comandos do sistema para mais informações..."
                  << std::endl;

        std::string cmd = "file '" + library_path + "'";
        std::cout << "$ " << cmd << std::endl;
        system (cmd.c_str());

        cmd = "ls -la '" + library_path + "'";
        std::cout << "$ " << cmd << std::endl;
        system (cmd.c_str());
    }

    ~MapleLibraryExplorer()
    {
        if (handle)
        {
            dlclose (handle);
            std::cout << "\nBiblioteca descarregada." << std::endl;
        }
    }
};

int main()
{
    std::cout << "=== EXPLORADOR DINÂMICO DA LIBMAPLE.SO ===" << std::endl;

    std::string maple_lib = "/opt/maple2021/bin.X86_64_LINUX/libmaple.so";

    MapleLibraryExplorer explorer (maple_lib);

    if (!explorer.loadLibrary())
    {
        return 1;
    }

    explorer.printLibraryInfo();
    explorer.probeFunctions();
    explorer.probeVariables();
    explorer.analyzeSymbols();
    explorer.testBasicExecution();

    std::cout << "\n=== EXPLORAÇÃO CONCLUÍDA ===" << std::endl;
    std::cout << "Para mais investigação:" << std::endl;
    std::cout << "1. Analise os arquivos maple_symbols.txt e maple_strings.txt"
              << std::endl;
    std::cout << "2. Procure por headers em /opt/maple2021/include" << std::endl;
    std::cout
            << "3. Use 'ltrace ./maple' para ver chamadas de função em tempo real"
            << std::endl;

    return 0;
}
