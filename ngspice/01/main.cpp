#include <iostream>
#include <vector>
#include <string>
#include <ngspice/sharedspice.h>

class NgSpiceSimulator {
private:
    bool initialized;
    
    // Callback functions para ngspice
    static int ng_getchar(char* outputreturn, int ident, void* userdata) {
        // Callback para entrada de caracteres
        return 0;
    }
    
    static int ng_getstat(char* outputreturn, int ident, void* userdata) {
        // Callback para status
        printf("Status: %s\n", outputreturn);
        return 0;
    }
    
    static int ng_exit(int exitstatus, bool immediate, bool quitexit, int ident, void* userdata) {
        // Callback para saída
        printf("NgSpice exit com status: %d\n", exitstatus);
        return exitstatus;
    }
    
    static int ng_thread_runs(bool noruns, int ident, void* userdata) {
        // Callback para threads
        return 0;
    }

public:
    NgSpiceSimulator() : initialized(false) {}
    
    bool initialize() {
        // Inicializa o ngspice
        int ret = ngSpice_Init(ng_getchar, ng_getstat, ng_exit, NULL, NULL, ng_thread_runs, this);
        
        if (ret != 0) {
            std::cerr << "Erro ao inicializar ngSpice: " << ret << std::endl;
            return false;
        }
        
        initialized = true;
        std::cout << "NgSpice inicializado com sucesso!" << std::endl;
        return true;
    }
    
    void runCircuit() {
        if (!initialized) {
            std::cerr << "NgSpice não inicializado!" << std::endl;
            return;
        }
        
        // Define o circuito SPICE
        // Exemplo: Divisor de tensão simples
        char* circuito[] = {
            (char*)"Divisor de Tensao Simples",
            (char*)"V1 1 0 DC 10V",           // Fonte de tensão de 10V
            (char*)"R1 1 2 1k",              // Resistor 1k entre nós 1 e 2
            (char*)"R2 2 0 2k",              // Resistor 2k entre nós 2 e 0
            (char*)".op",                     // Análise de ponto de operação
            (char*)".end",                    // Fim do netlist
            NULL
        };
        
        // Carrega o circuito
        std::cout << "Carregando circuito..." << std::endl;
        int result = ngSpice_Circ(circuito);
        
        if (result != 0) {
            std::cerr << "Erro ao carregar circuito: " << result << std::endl;
            return;
        }
        
        // Executa a simulação
        std::cout << "Executando simulação..." << std::endl;
        ngSpice_Command((char*)"run");
        
        // Mostra os resultados
        showResults();
    }
    
    void showResults() {
        // Obtém vetor de dados
        pvector_info* vecs = ngGet_Vec_Info((char*)"all");
        
        if (vecs == NULL) {
            std::cerr << "Nenhum dado de simulação encontrado!" << std::endl;
            return;
        }
        
        std::cout << "\n=== RESULTADOS DA SIMULAÇÃO ===" << std::endl;
        
        // Itera através dos vetores de dados
        for (int i = 0; vecs[i].vecname; i++) {
            pvecvalues* vec = ngGet_Vec_Info(vecs[i].vecname);
            
            if (vec && vec->vecsa) {
                std::cout << "Vetor: " << vecs[i].vecname << std::endl;
                std::cout << "Comprimento: " << vec->vecsa[0].veclen << std::endl;
                
                // Mostra alguns valores (limitado para não poluir a saída)
                int maxShow = std::min(5, vec->vecsa[0].veclen);
                for (int j = 0; j < maxShow; j++) {
                    if (vec->vecsa[0].vectype == 1) { // Real
                        std::cout << "  [" << j << "] = " << vec->vecsa[0].pdvec[j] << std::endl;
                    } else if (vec->vecsa[0].vectype == 2) { // Complex
                        std::cout << "  [" << j << "] = " << vec->vecsa[0].pdvec[2*j] 
                                  << " + j" << vec->vecsa[0].pdvec[2*j+1] << std::endl;
                    }
                }
                if (vec->vecsa[0].veclen > maxShow) {
                    std::cout << "  ... (mais " << (vec->vecsa[0].veclen - maxShow) << " valores)" << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
    
    void runTransientAnalysis() {
        if (!initialized) {
            std::cerr << "NgSpice não inicializado!" << std::endl;
            return;
        }
        
        // Circuito RC para análise transiente
        char* circuito[] = {
            (char*)"Circuito RC - Analise Transiente",
            (char*)"V1 1 0 PULSE(0 5 0 1n 1n 5u 10u)",  // Pulso de 0 a 5V
            (char*)"R1 1 2 1k",                           // Resistor 1k
            (char*)"C1 2 0 1u",                           // Capacitor 1µF
            (char*)".tran 0.1u 20u",                      // Análise transiente
            (char*)".end",
            NULL
        };
        
        std::cout << "Carregando circuito RC..." << std::endl;
        int result = ngSpice_Circ(circuito);
        
        if (result != 0) {
            std::cerr << "Erro ao carregar circuito RC: " << result << std::endl;
            return;
        }
        
        std::cout << "Executando análise transiente..." << std::endl;
        ngSpice_Command((char*)"run");
        
        // Obter dados específicos
        getSpecificData();
    }
    
    void getSpecificData() {
        // Obtém dados específicos de tensão no nó 2
        pvecvalues* voltageData = ngGet_Vec_Info((char*)"v(2)");
        pvecvalues* timeData = ngGet_Vec_Info((char*)"time");
        
        if (voltageData && timeData) {
            std::cout << "\n=== DADOS DE TENSÃO V(2) vs TEMPO ===" << std::endl;
            
            int numPoints = std::min(timeData->vecsa[0].veclen, voltageData->vecsa[0].veclen);
            int step = std::max(1, numPoints / 10); // Mostra apenas 10 pontos
            
            for (int i = 0; i < numPoints; i += step) {
                double time = timeData->vecsa[0].pdvec[i] * 1e6; // Converter para µs
                double voltage = voltageData->vecsa[0].pdvec[i];
                
                std::cout << "Tempo: " << time << " µs, V(2): " << voltage << " V" << std::endl;
            }
        }
    }
    
    ~NgSpiceSimulator() {
        if (initialized) {
            // Limpa recursos do ngspice
            ngSpice_Command((char*)"quit");
        }
    }
};

int main() {
    std::cout << "=== SIMULADOR NGSPICE C++ ===" << std::endl;
    
    NgSpiceSimulator simulator;
    
    // Inicializa o simulador
    if (!simulator.initialize()) {
        return -1;
    }
    
    // Exemplo 1: Análise DC (divisor de tensão)
    std::cout << "\n1. Executando análise DC..." << std::endl;
    simulator.runCircuit();
    
    // Exemplo 2: Análise transiente (circuito RC)
    std::cout << "\n2. Executando análise transiente..." << std::endl;
    simulator.runTransientAnalysis();
    
    std::cout << "\nSimulação concluída!" << std::endl;
    return 0;
}

