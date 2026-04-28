# Especificação da Implementação

> [!CAUTION]
> - Você <ins>**não pode utilizar ferramentas de IA para escrever esta
>   especificação**</ins>

## Integrantes da dupla

- **Aluno 1 - Nome**: Guilherme Hilgert Feroleto
- **Aluno 1 - Cartão UFRGS**: 587076

- **Aluno 2 - Nome**: Henrique Wermann da Silva
- **Aluno 2 - Cartão UFRGS**: 588786

## Detalhes do que será implementado

- **Título do trabalho**: Jogo de Luta
- **Parágrafo curto descrevendo o que será implementado**: Será implementado um jogo de luta com base no jogo Tekken 3, lançado em 1997. No jogo será possível controlar um dos personagens e lutar contra o adversário. O personagem terá sua movimentação e um conjunto de golpes pré definidos com base a atender todas as especificações do trabalho.

## Especificação visual

### Vídeo - Link

> [!IMPORTANT]
> - Coloque aqui um link para um vídeo que mostre a aplicação gráfica
>   de referência que você vai implementar. **Sua implementação deverá
>   ser o mais parecido possível com o que é mostrado no vídeo (mais
>   detalhes abaixo).**
> - **Você não pode escolher como referência: (1) algum trabalho realizado
>   por outros alunos desta disciplina, em semestres anteriores. (2) Minecraft.**
> - Por exemplo, você pode colocar um vídeo de um jogo que você gosta,
>   e seu trabalho final será uma re-implementação do jogo.
> - O vídeo pode ser um link para YouTube, Google Drive, ou arquivo mp4 dentro
>   do próprio repositório. Mas, garanta que qualquer um tenha
>   permissão de acesso ao vídeo através deste link.

[text](https://www.youtube.com/watch?v=ikzuz2wL1X0)

### Vídeo - Timestamp

> [!IMPORTANT]
> - Coloque aqui um **intervalo de ~30 segundos** do vídeo acima, que
>   será a base de comparação para avaliar se o seu trabalho final
>   conseguiu ou não reproduzir a referência.

- **Timestamp inicial**: 0
- **Timestamp final**: 30

### Imagens

> [!IMPORTANT]
> - Coloque aqui **três imagens** capturadas do vídeo acima, que você
>   irá usar como ilustração para as explicações que vêm abaixo.

<mark>`<preencher>`</mark>

## Especificação textual

Para cada um dos requisitos abaixo (detalhados no [Enunciado do Trabalho final - Moodle](https://moodle.ufrgs.br/mod/assign/view.php?id=6018620)), escreva um parágrafo **curto** explicando como este requisito será atendido, apontando itens específicos do vídeo/imagens que você incluiu acima que atendem estes requisitos.

### Malhas poligonais complexas
Os objetos virtuais com malhas poligonais complexas serão representados principalmente pelos modelos dos dois personagens. Além disso, o cenário também contará com elementos adicionais ao fundo, inspirados no vídeo de referência, contribuindo para a composição visual do ambiente.

### Transformações geométricas controladas pelo usuário
As transformações geométricas controladas pelo usuário estarão diretamente relacionadas à movimentação e às ações do personagem. Por meio de entradas do teclado, o usuário poderá alterar a posição, permitindo deslocamento. Além disso, os golpes executados também serão implementados como transformações geométricas, envolvendo animações que modificam a pose do personagem ao longo do tempo.

### Diferentes tipos de câmeras
Serão implementados dois tipos de câmera para visualização da cena: uma câmera do tipo look-at e uma câmera livre. A câmera look-at será utilizada para manter o foco automático nos personagens, posicionando-se de forma a acompanhar a ação do combate. Já a câmera livre permitirá ao usuário explorar a cena de maneira independente, podendo mover-se e rotacionar livremente pelo ambiente.

### Instâncias de objetos
Os dois personagens que estarão lutando serão renderizados diversas vezs com diferentes Model Matrix. Cada instância vai possuir sua própria Model Matrix, responsável por definir suas tranformações. Isso permite que o mesmo modelo represente múltiplos objetos distintos na cena.

### Testes de intersecção
Os testes de intersecção serão feitos a partir dos golpes dos personagens. Cada golpe precisa "acertar" a hitbox para contar, esses testes serão utilizados para essa verificação.

### Modelos de Iluminação em todos os objetos
O modelo de iluminação utilizado será o de Blinn-Phong, uma variação do modelo de Phong amplamente empregada em aplicações gráficas em tempo real. Esse modelo considera três componentes principais: ambiente, difusa e especular, permitindo simular de forma eficiente a interação da luz com as superfícies dos objetos. Com isso, será possível obter resultados visuais mais realistas nos personagens e no cenário, mantendo um bom desempenho na renderização.

### Mapeamento de texturas em todos os objetos
O mapeamento de texturas será aplicado nos personagens, no chão e nos elementos do ambiente, com o objetivo de enriquecer o nível de detalhe visual. Assim será possível, por exemplo, aplicar texturas de roupa nos personagens, padrões no chão da arena e detalhes em objetos do cenário.

### Movimentação com curva Bézier cúbica
A movimentação do projétil será implementada utilizando uma curva Bézier cúbica, permitindo um trajeto suave e controlado entre o ponto de origem e o alvo. Nesse caso, ao executar um golpe com a espada, o projétil não seguirá uma linha reta, mas sim uma trajetória curva definida por pontos de controle.

### Animações baseadas no tempo ($\Delta t$)
A movimentação dos objetos será baseada no tempo, garantindo que o comportamento da aplicação seja consistente independentemente do desempenho do computador. Para isso, as posições e transformações dos objetos serão atualizadas utilizando o tempo decorrido entre quadros (delta time). Dessa forma, a velocidade dos movimentos permanece constante, evitando que o jogo fique mais rápido ou mais lento em máquinas diferentes.

## Limitações esperadas

> [!IMPORTANT]
> - Coloque aqui uma lista de detalhes visuais ou de interação que
>   aparecem no vídeo e/ou imagens acima, mas que você **não pretende
>   implementar** ou que você **irá implementar parcialmente**.
> - Para cada item, **explique por que** não será implementado ou por
>   que será implementado parcialmente.

- os golpes dos personagens serão implementados parcialmente, iremos fazer golpes específicos para cumprir requisitos do trabalho, como usar projéteis com movimentos baseados em curvas de Bézier.
- A quantidade de golpes será restringida, pois não geram interações diferentes entre os personagens, além de dano e reação, e investiremos mais tempo para a implementação de outros aspectos do jogo.
- A movimentação dos personagens será parcialmente implementada, não iremos incluir os movimentos em que o personagem dá cambalhotas para se movimentar, nem as animações em que o personagem fica parado, por exemplo os taunts. Acreditamos que implentar as animações dos golpes já nos concede uma experiência de aprendizagem proveitosa, e implementar todas as animações do personagem desviaria a atenção de outros aspectos do projeto.
- Não serão implementados os textos que aparecem na tela, implementar a lógica de contador de combo e danos desvia do foco principal do trabalho. No lugar, implementaremos um mostrador da quantidade de vida restante do personagem.
