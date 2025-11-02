cmake_minimum_required(VERSION 3.20.0)

# Define versão do projeto para melhor rastreabilidade
project(
  GitClone
  VERSION 1.0.0
  LANGUAGES CXX)

# Define o padrão C++ (boas práticas modernas)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cria o executável
add_executable(welcome src/main.cpp)

# Copia arquivo de configuração para o diretório de build
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.yaml
               ${CMAKE_BINARY_DIR}/etc/config.yaml COPYONLY)

# Tenta encontrar yaml-cpp instalado no sistema
find_package(yaml-cpp QUIET)

if(NOT yaml-cpp_FOUND)
  message(STATUS "yaml-cpp not found, will clone from repository")

  # Verifica se Git está disponível
  find_package(Git REQUIRED)

  # Define o diretório onde será clonado
  set(YAML_CPP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/yaml-cpp)

  # Verifica se o diretório já existe (evita clone duplicado)
  if(NOT EXISTS ${YAML_CPP_DIR})
    message(STATUS "Cloning yaml-cpp into ${YAML_CPP_DIR}")

    # Cria o diretório extern se não existir
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extern)

    # Clona o repositório
    execute_process(
      COMMAND
        ${GIT_EXECUTABLE} clone --depth 1 # Clone raso (mais rápido)
        # --branch new-api # Versão específica (opcional)
        https://github.com/jbeder/yaml-cpp.git ${YAML_CPP_DIR}
      RESULT_VARIABLE GIT_RESULT
      ERROR_VARIABLE GIT_ERROR)

    # Verifica se o clone foi bem-sucedido
    if(NOT GIT_RESULT EQUAL 0)
      message(FATAL_ERROR "Failed to clone yaml-cpp: ${GIT_ERROR}")
    endif()
  else()
    message(STATUS "yaml-cpp directory already exists, skipping clone")
  endif()

  # Desabilita testes e exemplos do yaml-cpp (compilação mais rápida)
  set(YAML_CPP_BUILD_TESTS
      OFF
      CACHE BOOL "" FORCE)
  set(YAML_CPP_BUILD_TOOLS
      OFF
      CACHE BOOL "" FORCE)
  set(YAML_CPP_BUILD_CONTRIB
      OFF
      CACHE BOOL "" FORCE)

  # Adiciona o subdiretório
  add_subdirectory(${YAML_CPP_DIR})
endif()

# Vincula a biblioteca ao executável
target_link_libraries(welcome PRIVATE yaml-cpp::yaml-cpp)

include(CMakePrintHelpers)
cmake_print_properties(TARGETS yaml-cpp PROPERTIES TYPE SOURCE_DIR)

# eof
