#include <iostream>
#include <ngspice/sharedspice.h>
#include <string>
#include <vector>

class NgSpiceSimulator
{
private:
    bool initialized;

    // Callback functions para ngspice
    static int ng_getchar (char* outputreturn, int ident, void* userdata)
    {
        // Callback para entrada de caracteres
        return 0;
    }

    static int ng_getstat (char* outputreturn, int ident, void* userdata)
    {
        // Callback para status
        printf ("Status: %s\n", outputreturn);
        return 0;
    }

    static int ng_exit (int exitstatus, bool immediate, bool quitexit, int ident,
                        void* userdata)
    {
        // Callback para saída
        printf ("NgSpice exit com status: %d\n", exitstatus);
        return exitstatus;
    }

    static int ng_thread_runs (bool noruns, int ident, void* userdata)
    {
        // Callback para threads
        return 0;
    }

public:
    NgSpiceSimulator() : initialized (false) {}

    bool initialize()
    {
        // Inicializa o ngspice
        int ret = ngSpice_Init (ng_getchar, ng_getstat, ng_exit, NULL, NULL,
                                ng_thread_runs, this);

        if (ret != 0)
        {
            std::cerr << "Erro ao inicializar ngSpice: " << ret << std::endl;
            return false;
        }

        initialized = true;
        std::cout << "NgSpice inicializado com sucesso!" << std::endl;
        return true;
    }

    void runCircuit()
    {
        if (!initialized)
        {
            std::cerr << "NgSpice não inicializado!" << std::endl;
            return;
        }

        // Define o circuito SPICE
        // Exemplo: Divisor de tensão simples
        char* circuito[] = { (char*)"Divisor de Tensao Simples",
                             (char*)"V1 1 0 DC 10V",  // Fonte de tensão de 10V
                             (char*)"R1 1 2 1k",      // Resistor 1k entre nós 1 e 2
                             (char*)"R2 2 0 2k",      // Resistor 2k entre nós 2 e 0
                             (char*)".op",            // Análise de ponto de operação
                             (char*)".end",           // Fim do netlist
                             NULL
                           };

        // Carrega o circuito
        std::cout << "Carregando circuito..." << std::endl;
        int result = ngSpice_Circ (circuito);

        if (result != 0)
        {
            std::cerr << "Erro ao carregar circuito: " << result << std::endl;
            return;
        }

        // Executa a simulação
        std::cout << "Executando simulação..." << std::endl;
        ngSpice_Command ((char*)"run");

        // Mostra os resultados usando uma abordagem mais simples
        showResults();
    }

    void showResults()
    {
        std::cout << "\n=== RESULTADOS DA SIMULAÇÃO ===" << std::endl;

        // Lista todos os plots disponíveis
        char** plots = ngSpice_AllPlots();
        if (plots)
        {
            std::cout << "Plots disponíveis:" << std::endl;
            for (int i = 0; plots[i]; i++)
            {
                std::cout << "  - " << plots[i] << std::endl;

                // Para cada plot, lista os vetores
                char** vecs = ngSpice_AllVecs (plots[i]);
                if (vecs)
                {
                    std::cout << "    Vetores:" << std::endl;
                    for (int j = 0; vecs[j]; j++)
                    {
                        std::cout << "      * " << vecs[j] << std::endl;

                        // Tenta obter informações básicas do vetor
                        pvector_info vecInfo = ngGet_Vec_Info (vecs[j]);
                        if (vecInfo)
                        {
                            std::cout << "        Nome: " << vecInfo->v_name << std::endl;
                            std::cout << "        Comprimento: " << vecInfo->v_length
                                      << std::endl;
                            std::cout << "        Tipo: " << vecInfo->v_type << std::endl;
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
        else
        {
            std::cout << "Nenhum plot encontrado!" << std::endl;
        }

        // Tenta obter alguns valores específicos usando comandos ngspice
        std::cout << "Executando comandos para mostrar valores:" << std::endl;
        ngSpice_Command ((char*)"print all");
        ngSpice_Command ((char*)"print v(1) v(2)");
    }

    void runTransientAnalysis()
    {
        if (!initialized)
        {
            std::cerr << "NgSpice não inicializado!" << std::endl;
            return;
        }

        // Circuito RC para análise transiente
        char* circuito[] =
        {
            (char*)"Circuito RC - Analise Transiente",
            (char*)"V1 1 0 PULSE(0 5 0 1n 1n 5u 10u)",  // Pulso de 0 a 5V
            (char*)"R1 1 2 1k",                         // Resistor 1k
            (char*)"C1 2 0 1u",                         // Capacitor 1µF
            (char*)".tran 0.1u 20u",                    // Análise transiente
            (char*)".end",
            NULL
        };

        std::cout << "Carregando circuito RC..." << std::endl;
        int result = ngSpice_Circ (circuito);

        if (result != 0)
        {
            std::cerr << "Erro ao carregar circuito RC: " << result << std::endl;
            return;
        }

        std::cout << "Executando análise transiente..." << std::endl;
        ngSpice_Command ((char*)"run");

        // Mostra informações sobre a análise transiente
        getTransientInfo();
    }

    void getTransientInfo()
    {
        std::cout << "\n=== INFORMAÇÕES DA ANÁLISE TRANSIENTE ===" << std::endl;

        // Lista plots e vetores da análise transiente
        char** plots = ngSpice_AllPlots();
        if (plots)
        {
            for (int i = 0; plots[i]; i++)
            {
                std::cout << "Plot: " << plots[i] << std::endl;

                char** vecs = ngSpice_AllVecs (plots[i]);
                if (vecs)
                {
                    for (int j = 0; vecs[j]; j++)
                    {
                        pvector_info vecInfo = ngGet_Vec_Info (vecs[j]);
                        if (vecInfo && vecInfo->v_length > 0)
                        {
                            std::cout << "  Vetor: " << vecInfo->v_name
                                      << " (pontos: " << vecInfo->v_length << ")"
                                      << std::endl;
                        }
                    }
                }
            }
        }

        // Usa comandos ngspice para mostrar alguns pontos
        std::cout << "\nAlguns valores da simulação:" << std::endl;
        ngSpice_Command ((char*)"print v(1)[0] v(2)[0] time[0]"); // Primeiro ponto
        ngSpice_Command ((char*)"print v(1)[10] v(2)[10] time[10]"); // 10º ponto
    }

    // Método auxiliar para executar comandos personalizados
    void executeCommand (const std::string& command)
    {
        if (!initialized)
        {
            std::cerr << "NgSpice não inicializado!" << std::endl;
            return;
        }

        std::cout << "Executando: " << command << std::endl;
        ngSpice_Command ((char*)command.c_str());
    }

    ~NgSpiceSimulator()
    {
        if (initialized)
        {
            // Limpa recursos do ngspice
            ngSpice_Command ((char*)"quit");
        }
    }
};

int main()
{
    std::cout << "=== SIMULADOR NGSPICE C++ ===" << std::endl;

    NgSpiceSimulator simulator;

    // Inicializa o simulador
    if (!simulator.initialize())
    {
        return -1;
    }

    // Exemplo 1: Análise DC (divisor de tensão)
    std::cout << "\n1. Executando análise DC..." << std::endl;
    simulator.runCircuit();

    // Exemplo 2: Análise transiente (circuito RC)
    std::cout << "\n2. Executando análise transiente..." << std::endl;
    simulator.runTransientAnalysis();

    // Exemplo 3: Comandos personalizados
    std::cout << "\n3. Executando comandos personalizados..." << std::endl;
    simulator.executeCommand ("show all");
    simulator.executeCommand ("display");

    std::cout << "\nSimulação concluída!" << std::endl;
    return 0;
}
