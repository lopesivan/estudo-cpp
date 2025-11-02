cmake_minimum_required(VERSION 3.20.0)

# Define projeto com versão
project(
  ExternalProjectGit
  VERSION 1.0.0
  LANGUAGES CXX)

# Define padrão C++ moderno
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cria o executável
add_executable(welcome main.cpp)

# Copia arquivo de configuração com caminhos explícitos
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.yaml
               ${CMAKE_BINARY_DIR}/config.yaml COPYONLY)

# Inclui módulo para projetos externos
include(ExternalProject)

# Define diretórios para o projeto externo
set(YAML_CPP_PREFIX ${CMAKE_BINARY_DIR}/external/yaml-cpp)
set(YAML_CPP_INSTALL_DIR ${YAML_CPP_PREFIX}/install)
set(YAML_CPP_INCLUDE_DIR ${YAML_CPP_INSTALL_DIR}/include)
set(YAML_CPP_LIB_DIR ${YAML_CPP_INSTALL_DIR}/lib)

# Adiciona yaml-cpp como projeto externo
ExternalProject_Add(
  external-yaml-cpp
  # Configuração do repositório
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  # GIT_TAG yaml-cpp-0.8.0 # Versão mais recente
  GIT_SHALLOW TRUE # Clone raso (mais rápido)
  # Diretórios
  PREFIX ${YAML_CPP_PREFIX}
  INSTALL_DIR ${YAML_CPP_INSTALL_DIR}
  # Configurações do CMake para yaml-cpp
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${YAML_CPP_INSTALL_DIR}
             -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DYAML_CPP_BUILD_TESTS=OFF # Desabilita testes
             -DYAML_CPP_BUILD_TOOLS=OFF # Desabilita ferramentas
             -DYAML_CPP_BUILD_CONTRIB=OFF # Desabilita contrib
             -DCMAKE_POSITION_INDEPENDENT_CODE=ON
  # Configuração de log (útil para debug)
  LOG_DOWNLOAD TRUE
  LOG_CONFIGURE TRUE
  LOG_BUILD TRUE
  LOG_INSTALL TRUE)

# Cria uma biblioteca IMPORTED para representar yaml-cpp
add_library(yaml-cpp STATIC IMPORTED)

# Define as propriedades da biblioteca importada
set_target_properties(
  yaml-cpp
  PROPERTIES
    IMPORTED_LOCATION
    ${YAML_CPP_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}yaml-cpp${CMAKE_STATIC_LIBRARY_SUFFIX}
    INTERFACE_INCLUDE_DIRECTORIES ${YAML_CPP_INCLUDE_DIR})

# Garante que o projeto externo seja compilado antes do executável
add_dependencies(welcome external-yaml-cpp)

# Agora podemos linkar yaml-cpp corretamente
target_link_libraries(welcome PRIVATE yaml-cpp)

# Utilidades de debug do CMake
include(CMakePrintHelpers)

# Imprime informações sobre o build (apenas para debug)
message(STATUS "=== Build Information ===")
cmake_print_variables(CMAKE_BUILD_TYPE CMAKE_CXX_COMPILER_ID
                      CMAKE_CXX_COMPILER_VERSION)

# Imprime diretórios do yaml-cpp
message(STATUS "yaml-cpp install dir: ${YAML_CPP_INSTALL_DIR}")
message(STATUS "yaml-cpp include dir: ${YAML_CPP_INCLUDE_DIR}")
message(STATUS "yaml-cpp lib dir: ${YAML_CPP_LIB_DIR}")

# Imprime propriedades do executável
cmake_print_properties(TARGETS welcome PROPERTIES LINK_LIBRARIES
                                                  INCLUDE_DIRECTORIES)
