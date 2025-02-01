# CMake mínimo necessário
cmake_minimum_required(VERSION 3.13)

# Configuração do padrão C
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Caminho para o SDK do Raspberry Pi Pico
set(PICO_SDK_PATH "C:/Users/regad/.pico-sdk/sdk/2.1.0")

# Garantir que o caminho esteja correto
message(STATUS "PICO_SDK_PATH: ${PICO_SDK_PATH}")

# Definir o tipo de placa (pico_w no caso)
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Incluir o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

# Inicializar o SDK do Raspberry Pi Pico
pico_sdk_init()

# Definir o nome do projeto
project(Tarefa_botoes C CXX ASM)

# Adicionar o executável
add_executable(Tarefa_botoes Tarefa-botoes.c)

# Definir o nome e versão do programa
pico_set_program_name(Tarefa_botoes "Tarefa_botoes")
pico_set_program_version(Tarefa_botoes "0.1")

# Habilitar saída por USB
pico_enable_stdio_uart(Tarefa_botoes 0)
pico_enable_stdio_usb(Tarefa_botoes 1)

# Adicionar cabeçalhos PIO
pico_generate_pio_header(Tarefa_botoes ${CMAKE_CURRENT_LIST_DIR}/pio_matrix.pio)

# Adicionar fontes ao executável
target_sources(Tarefa_botoes PRIVATE Tarefa-botoes.c)

# Adicionar a biblioteca padrão do Pico e outras bibliotecas necessárias
target_link_libraries(Tarefa_botoes PRIVATE
        pico_stdlib
        hardware_pio
        hardware_adc
        pico_bootrom
)

# Incluir arquivos adicionais para o projeto
target_include_directories(Tarefa_botoes PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Adicionar qualquer biblioteca adicional que o usuário queira
target_link_libraries(Tarefa_botoes 
        # Aqui você pode adicionar outras bibliotecas, se necessário
)

# Adicionar saídas extras do Pico
pico_add_extra_outputs(Tarefa_botoes)
