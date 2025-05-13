# Nome do módulo (sem extensão)
MODULE := meucodigo

# Diretórios de inclusão do Octave
OCTAVE_VERSION := 8.4.0
INCLUDES := -I/usr/include/octave-$(OCTAVE_VERSION)/octave/.. -I/usr/include/octave-$(OCTAVE_VERSION)/octave

# Flags de compilação
CXX := g++
CXXFLAGS := -c -Wdate-time -D_FORTIFY_SOURCE=3 -fPIC $(INCLUDES) \
  -pthread -fopenmp -g -O2 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer \
  -flto=auto \
  -ffat-lto-objects -fstack-protector-strong -fstack-clash-protection \
  -Wformat -Werror=format-security -fcf-protection


# Flags de link
LDFLAGS := -shared -Wl,-Bsymbolic -Wl,-Bsymbolic-functions -flto=auto -ffat-lto-objects -Wl,-z,relro
LIBS := -L/usr/lib/x86_64-linux-gnu

# Alvo principal
all: $(MODULE).oct

# Compilação
$(MODULE).o: $(MODULE).cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Linkagem
$(MODULE).oct: $(MODULE).o
	$(CXX) $(INCLUDES) -pthread -fopenmp -g -O2 -fno-omit-frame-pointer \
  -mno-omit-leaf-frame-pointer \
  -flto=auto -ffat-lto-objects -fstack-protector-strong -fstack-clash-protection \
  -Wformat -Werror=format-security -fcf-protection \
  -o $@ $< $(LDFLAGS) $(LIBS)

# Limpeza
clean:
	rm -f $(MODULE).o $(MODULE).oct

run: meucodigo.oct
	octave usage.oc

# Ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make         -> Compila $(MODULE).cpp em $(MODULE).oct"
	@echo "  make clean   -> Remove arquivos intermediários e o módulo compilado"
