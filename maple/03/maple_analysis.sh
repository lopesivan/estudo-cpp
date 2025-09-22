#!/usr/bin/env bash

# Script para explorar a biblioteca libmaple.so do Maple 2021
# Baseado na análise das dependências descobertas

echo "=== ANÁLISE DA BIBLIOTECA LIBMAPLE.SO ==="
echo

MAPLE_PATH="/opt/maple2021/bin.X86_64_LINUX"
MAPLE_LIB="$MAPLE_PATH/libmaple.so"

if [ ! -f "$MAPLE_LIB" ]; then
    echo "Erro: $MAPLE_LIB não encontrado!"
    exit 1
fi

cd "$MAPLE_PATH"

echo "1. INFORMAÇÕES BÁSICAS DA BIBLIOTECA"
echo "======================================"
file "$MAPLE_LIB"
echo
ls -la "$MAPLE_LIB"
echo

echo "2. SÍMBOLOS EXPORTADOS (FUNÇÕES PÚBLICAS)"
echo "========================================="
echo "Extraindo símbolos com nm (apenas funções públicas):"
nm -D "$MAPLE_LIB" 2>/dev/null | grep " T " | head -20
echo "... (mostrando apenas as primeiras 20)"
echo

echo "Total de símbolos exportados:"
nm -D "$MAPLE_LIB" 2>/dev/null | grep " T " | wc -l
echo

echo "3. STRINGS INTERESSANTES NA BIBLIOTECA"
echo "====================================="
echo "Procurando por strings que podem indicar APIs:"
strings "$MAPLE_LIB" | grep -E "(init|Init|INIT)" | head -10
echo
strings "$MAPLE_LIB" | grep -E "(eval|Eval|EVAL)" | head -10
echo
strings "$MAPLE_LIB" | grep -E "(maple|Maple|MAPLE)" | head -10
echo

echo "4. SEÇÕES DA BIBLIOTECA"
echo "======================"
objdump -h "$MAPLE_LIB" | head -20
echo

echo "5. DEPENDÊNCIAS DETALHADAS"
echo "=========================="
echo "Bibliotecas Intel Math (libimf.so, libsvml.so):"
ldd "$MAPLE_LIB" | grep -E "(imf|svml|irng)"
echo
echo "Bibliotecas Boost customizadas:"
ldd "$MAPLE_LIB" | grep boost
echo
echo "Bibliotecas ICU (Unicode):"
ldd "$MAPLE_LIB" | grep icu
echo

echo "6. PROCURANDO POR PADRÕES DE API"
echo "==============================="
echo "Funções que podem ser de inicialização:"
nm -D "$MAPLE_LIB" 2>/dev/null | grep -i -E "(start|begin|init|create|new)" | head -10
echo
echo "Funções que podem ser de avaliação:"
nm -D "$MAPLE_LIB" 2>/dev/null | grep -i -E "(eval|exec|run|compute|calc)" | head -10
echo
echo "Funções que podem ser de cleanup:"
nm -D "$MAPLE_LIB" 2>/dev/null | grep -i -E "(stop|end|finish|destroy|delete|free|clean)" | head -10
echo

echo "7. ANÁLISE DE VERSIONING"
echo "======================="
echo "Procurando por informações de versão:"
strings "$MAPLE_LIB" | grep -E "(version|Version|VERSION)" | head -5
echo
strings "$MAPLE_LIB" | grep -E "([0-9]+\.[0-9]+\.[0-9]+)" | head -5
echo

echo "8. POSSÍVEIS INTERFACES C/C++"
echo "============================="
echo "Símbolos que parecem ser interfaces C (não mangled):"
nm -D "$MAPLE_LIB" 2>/dev/null | grep " T " | grep -v "::" | head -15
echo
echo "Símbolos C++ (mangled):"
nm -D "$MAPLE_LIB" 2>/dev/null | grep " T " | grep "::" | head -10
echo

echo "9. SALVANDO LISTA COMPLETA DE SÍMBOLOS"
echo "======================================"
echo "Salvando todos os símbolos em maple_symbols.txt..."
nm -D "$MAPLE_LIB" 2>/dev/null >maple_symbols.txt
echo "Salvando strings em maple_strings.txt..."
strings "$MAPLE_LIB" >maple_strings.txt
echo "Arquivos salvos no diretório atual."
echo

echo "10. RECOMENDAÇÕES PARA PRÓXIMOS PASSOS"
echo "====================================="
echo "Para investigar mais profundamente:"
echo "- Use 'objdump -T $MAPLE_LIB' para ver símbolos dinâmicos"
echo "- Use 'readelf -a $MAPLE_LIB' para informações detalhadas do ELF"
echo "- Procure por headers (.h) no diretório /opt/maple2021/include"
echo "- Analise o executável 'maple' para ver como ele usa a biblioteca"
echo "- Use 'ltrace' para interceptar chamadas de biblioteca durante execução"
echo
