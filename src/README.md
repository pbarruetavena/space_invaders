> Por Pedro Gabriel Barruetavena Vieira

## SPACE INVADERS

### Como jogar?

Para jogar é simples, o código está em um arquivo para facilitar. Primeiramente baixe esse diretório. Feito isso, inclua o allegro-5.0.10-monolith-mt.dll no diretório (caso tenha baixado a versão que já o inclua isso não é necessário). Ajuste o Makefile (com sua versão do allegro e mingw e seus respectivos caminhos). Depois disso é só compilar com o make no diretório correto.

No jogo principal basta mover com as teclas < > ou A e D. Use space para atirar. Para ganhar destrua todos os aliens. Se algum alien chegar em você ou na superficie você perde.

O jogo tem um menu principal e um ranking. Ao jogar, seu resultado aparece na tela e, caso esteja entre os 5 melhores, irá para o ranking com o nome que você pode escolher digitando na tela que pede o nome (pode usar backspace para apagar). Apertando enter você volta para o menu e pode seguir as opções da tela com suas respectivas teclas.

> Informações de implementação

### Estruturas

Foi usada 4 estruturas, para facilitar a representação dos elementos: O alien, a nave e o tiro guardam as coordenadas, e os dois primeiros ainda guardam um ponteiro para sua imagem. Já o registro do ranking, diferente das outras, só organiza as informações para construção do ranking.

### Colisões 

As colisões foram implementadas de forma a simplificar o desenvolvimento. Os aliens e o tiro foram tratados com circunferências, dessa forma, tratar a colisão entre eles foi simples, pois basta ver se a diferença entre os centros é menor que a soma dos raios. Já a nave, por ter um formato próximo ao triangular, para detectar colisão, eu simplifiquei de uma forma diferente: considerei duas circunferências: uma com centro canto inferior direiro e outra no esquerda, ambas passando pelo topo central da nave. Assim cada uma cobria a detecção do lado contráro (em relação à nave) do seu centro.

### Fluxo do código

Para construir o menu, o código guarda uma variável action, que representa a opção que está sendo executada no momento, podendo ser desde PLAYING até RANKING. A partir dela, dentro da estrutura do allegro, verifico qual está selecionada e construo o fluxo do código. É interessante perceber que o jogo efetivo, ao ser iniciado, aloca tudo e mantém nas variáveis da função, e ao ser finalizado, desaloca tudo, assim não é necessário desalocar os itens quando sair do loop principal do allegro.

### Funções

Apesar de estar em um mesmo arquivo, as funções são auto-explicativas e cuidam de toda a lógica do jogo. As funções de load alocam recurso e inicializam os elementos, as funções display constroem a tela, as funções destroy desalocam recursos, e assim por diante.

### Outros

Você pode alterar as configurações gerais do jogo se desejar (por fora da interface), como velocidade dos aliens, tiro e da nave, basta mudar nas definições iniciais