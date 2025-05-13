# Compilador e flags
V        := 14
CXX      := g++-$(V)
CXXFLAGS := -std=c++20 -fmodules-ts -Wall -O2

# Arquivos
MODULE     := math_utils.ixx
MODULE_OBJ := math_utils.o
MAIN       := main.cpp
TARGET     := main

# Regra principal
all: $(TARGET)

# Compila o módulo (forçando tipo -x c++)
$(MODULE_OBJ): $(MODULE)
	$(CXX) $(CXXFLAGS) -x c++ -c $< -o $@

# Compila o programa principal com o módulo
$(TARGET): $(MAIN) $(MODULE_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Limpeza
clean:
	rm -rf gcm.cache
	rm -f $(TARGET) *.o

# Ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make        -> Compila o programa com módulo math_utils.ixx"
	@echo "  make clean  -> Remove arquivos gerados"
	@echo "  make help   -> Mostra esta ajuda"
