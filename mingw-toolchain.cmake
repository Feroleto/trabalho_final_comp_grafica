# MinGW UCRT64 Toolchain
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Especificar os compiladores com caminho completo
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Não procurar programas em /usr
set(CMAKE_FIND_ROOT_PATH "C:/msys64/ucrt64")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


