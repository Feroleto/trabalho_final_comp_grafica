1. Parte dos sistemas de colisão e input vêm de um código em java de um dos alunos. Para passar este código para c++ foi utilizado IA.

2. Para fazer as projeções das sombras dos personagens, foi utilizado o prompt "vamos tentar usar planar projection shadows, fazendo os personagens serem desenhados no chão de forma simplificada". A partir disso, a implementação foi baseada nos códigos e sugestões dela, com alterações no main.cpp, shader_vertex_animated.glsl e shader_fragment_animated.glsl.

3. Na parte da criação do skybox para o cenário foi utilizada a IA, tanto para ajudar na decisão do que usar, pois estavamos na dúvida entre usar apenas skybox e aplicar a textura diretamente nele, do jeito que fizemos, ou utilizar também um cubemap, mas optamos pela utilização apenas da skybox com a textura sendo aplicada diretamente.

4. Para a aplicação das animações foi utilizado IA principalmente para ajudar em funções auxiliares, como as funções "getBoneNames()" e "getBonePosition()", que ajudam principalmente nessa parte da seleção dos bones para aplicar as animações de maneira correta.

5. Na parte das texturas, foi utilizada a IA como ferramente de ajuda para conseguir comandos que ajudassem a ver todas as partes do modelo 3D do personagem e onde deveria aplicar cada uma de suas texturas.