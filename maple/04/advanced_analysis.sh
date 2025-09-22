#!/usr/bin/env bash

# Análise avançada da libmaple.so
# Baseado nas descobertas das dependências

MAPLE_PATH="/opt/maple2021/bin.X86_64_LINUX"
MAPLE_LIB="$MAPLE_PATH/libmaple.so"
MAPLE_EXE="$MAPLE_PATH/maple"

echo "=== ANÁLISE AVANÇADA DA BIBLIOTECA MAPLE ==="
echo "Diretório: $MAPLE_PATH"
echo

# 1. Análise do executável principal do Maple
echo "1. ANÁLISE DO EXECUTÁVEL MAPLE"
echo "=============================="
if [ -f "$MAPLE_EXE" ]; then
    echo "Tipo do arquivo:"
    file "$MAPLE_EXE"
    echo

    echo "Bibliotecas usadas pelo maple:"
    ldd "$MAPLE_EXE" | grep -v "not found"
    echo

    echo "Símbolos indefinidos no maple (que podem vir da libmaple.so):"
    nm -u "$MAPLE_EXE" 2>/dev/null | head -20
    echo "... (mostrando apenas os primeiros 20)"
    echo
else
    echo "Executável maple não encontrado em $MAPLE_EXE"
fi

# 2. Procura por arquivos de header
echo "2. PROCURA POR ARQUIVOS DE HEADER"
echo "================================="
INCLUDE_DIRS=(
    "/opt/maple2021/include"
    "/opt/maple2021/extern/include"
    "/opt/maple2021/java/externalcall"
    "$MAPLE_PATH/../include"
)

for dir in "${INCLUDE_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        echo "Encontrado diretório: $dir"
        find "$dir" -name "*.h" -o -name "*.hpp" 2>/dev/null | head -10
        echo
    fi
done

# 3. Análise detalhada dos símbolos
echo "3. ANÁLISE DETALHADA DE SÍMBOLOS"
echo "==============================="
cd "$MAPLE_PATH"

echo "Símbolos que começam com 'maple' (case insensitive):"
nm -D "$MAPLE_LIB" 2>/dev/null | grep -i "maple" | head -15
echo

echo "Símbolos que começam com maiúscula (possíveis APIs públicas):"
nm -D "$MAPLE_LIB" 2>/dev/null | grep " T " | grep "^[0-9a-f]* [A-Z]" | head -15
echo

echo "Símbolos relacionados a 'eval':"
nm -D "$MAPLE_LIB" 2>/dev/null | grep -i "eval" | head -10
echo

echo "Símbolos relacionados a 'init':"
nm -D "$MAPLE_LIB" 2>/dev/null | grep -i "init" | head -10
echo

# 4. Análise das strings para encontrar APIs
echo "4. ANÁLISE DE STRINGS PARA IDENTIFICAR APIs"
echo "=========================================="
echo "Strings que parecem ser nomes de função:"
strings "$MAPLE_LIB" | grep -E "^[a-zA-Z_][a-zA-Z0-9_]*$" | grep -E "(init|eval|start|stop|new|create|destroy)" | head -15
echo

echo "Mensagens de erro que podem revelar funções:"
strings "$MAPLE_LIB" | grep -i "error" | head -10
echo

echo "Strings relacionadas ao Maple:"
strings "$MAPLE_LIB" | grep -i "maple" | head -15
echo

# 5. Análise das seções ELF
echo "5. ANÁLISE DAS SEÇÕES ELF"
echo "========================"
echo "Seções principais:"
readelf -S "$MAPLE_LIB" | grep -E "\.(text|data|rodata|bss|init|fini|ctors|dtors)"
echo

echo "Tabela de símbolos dinâmicos (amostra):"
readelf -s "$MAPLE_LIB" | grep "FUNC.*GLOBAL.*DEFAULT" | head -15
echo

# 6. Investigação específica baseada nas dependências encontradas
echo "6. INVESTIGAÇÃO BASEADA NAS DEPENDÊNCIAS"
echo "======================================="
echo "A biblioteca usa Intel Math Library (libimf.so), isso sugere:"
echo "- Operações matemáticas de alta performance"
echo "- Possível interface para cálculos numéricos"
echo

echo "A biblioteca usa Boost customizado, isso sugere:"
echo "- Interface C++ moderna"
echo "- Possível uso de threads, filesystem, regex"
echo

echo "A biblioteca usa ICU (Unicode), isso sugere:"
echo "- Suporte a internacionalização"
echo "- Manipulação de strings Unicode"
echo

# 7. Análise de versioning e metadados
echo "7. VERSIONING E METADADOS"
echo "========================"
echo "Procurando por informações de versão nos binários:"
strings "$MAPLE_LIB" "$MAPLE_EXE" 2>/dev/null | grep -E "([0-9]+\.[0-9]+)" | sort -u | head -10
echo

echo "Procurando por copyright/vendor info:"
strings "$MAPLE_LIB" | grep -i -E "(copyright|maplesoft|waterloo)" | head -5
echo

# 8. Criação de um mapa de dependências
echo "8. MAPA DE DEPENDÊNCIAS"
echo "======================"
echo "Criando arquivo dependency_map.txt..."
{
    echo "=== MAPA DE DEPENDÊNCIAS DA LIBMAPLE.SO ==="
    echo "Data: $(date)"
    echo
    echo "Dependências diretas:"
    ldd "$MAPLE_LIB" | sort
    echo
    echo "Símbolos importados (amostra):"
    nm -u "$MAPLE_LIB" 2>/dev/null | head -50
} >dependency_map.txt
echo "Arquivo dependency_map.txt criado."
echo

# 9. Sugestões para reverse engineering
echo "9. PRÓXIMOS PASSOS PARA REVERSE ENGINEERING"
echo "=========================================="
echo "Para continuar a investigação:"
echo "1. Use 'objdump -d $MAPLE_LIB | less' para ver assembly"
echo "2. Use 'gdb $MAPLE_EXE' e coloque breakpoints nas funções"
echo "3. Use 'ltrace -e '*maple*' $MAPLE_EXE' para rastrear chamadas"
echo "4. Procure por documentação online sobre Maple OpenAPI ou External Calling"
echo "5. Analise scripts .mws ou .maple para ver como o Maple é usado"
echo

# 10. Tentativa de encontrar exemplos de uso
echo "10. PROCURA POR EXEMPLOS NO SISTEMA"
echo "==================================="
echo "Procurando por arquivos de exemplo no diretório do Maple:"
find /opt/maple2021 -name "*.c" -o -name "*.cpp" -o -name "*.h" 2>/dev/null | head -10
echo

echo "Procurando por arquivos README ou documentação:"
find /opt/maple2021 -name "*README*" -o -name "*readme*" -o -name "*HOWTO*" 2>/dev/null | head -5
echo

echo "=== ANÁLISE CONCLUÍDA ==="
echo "Arquivos gerados:"
echo "- maple_symbols.txt (símbolos da biblioteca)"
echo "- maple_strings.txt (strings da biblioteca)"
echo "- dependency_map.txt (mapa de dependências)"
echo
