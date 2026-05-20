# Computação Gráfica e Visualização I (INF01047) - INF/UFRGS

Este repositório contém o código base para o trabalho final. O enunciado completo do trabalho final está no Moodle:

https://moodle.ufrgs.br/mod/assign/view.php?id=6018620

## Instalação da biblioteca Assimp

Se estiver usando UCRT64 com MSYS2, seguir os seguintes passos:

```bash
abrir o terminal MSYS2 UCRT64 e rodar o seguinte comando:
pacman -S mingw-w64-ucrt-x86_64-assimp

após instalar, rodar o comando:
find /ucrt64 -name "libassimp*"

deve aparecer algo como:
/ucrt64/bin/libassimp-6.dll
/ucrt64/lib/libassimp.dll.a
```

# Configuração do CMakeLists.txt

verificar o target_include_directories

```bash
target_include_directories(${EXECUTABLE_NAME} BEFORE PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    # caminho padrão do MSYS2 UCRT64
    C:/msys64/ucrt64/include
)

se a assimp tiver sido instalada em um local diferente de C:/msys64/ucrt64/include, adicionar o caminho dentro do target_include_directories
```

adicionar link da biblioteca em target_link_libraries

```bash
target_link_libraries(${EXECUTABLE_NAME} 
    ${LIBGLFW} 
    gdi32 
    opengl32
    C:/msys64/ucrt64/lib/libassimp.dll.a
)

se quando rodou o comando find /ucrt64 -name "libassimp*" o caminho foi diferente de "/ucrt64/lib/libassimp.dll.a", adicionar o caminho completo dentro do target_link_libraries
```

# Criação do arquivo .vscode/c_cpp_properties.json

caso esteja dando problema nos includes das bibliotecas do assimp, criar um arquivo chamado c_cpp_properties_json dentro da pasta .vscode com o seguinte conteúdo:

```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/include",
                "C:/msys64/ucrt64/include",
                "C:/msys64/ucrt64/include/assimp",
                "${workspaceFolder}/include/glad",
                "${workspaceFolder}/include/GLFW",
                "${workspaceFolder}/include/glm",
                "${workspaceFolder}/include/KHR"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.20348.0",
            "compilerPath": "C:/msys64/ucrt64/bin/g++.exe",
            "cStandard": "c17",
            "cppStandard": "c++11",
            "intelliSenseMode": "gcc-x64"
        }
    ],
    "version": 4
}
```