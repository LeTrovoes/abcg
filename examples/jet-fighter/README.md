# Jet Fighter

## Sobre
Um jogo multiplayer desenvolvido em C++, utilizando OpenGL juntamente com a biblioteca ABCg, para a disciplina de Computação Gráfica. Turma A1, diurno (DA1MCTA008-17SA), 3º quadrimestre de 2021.

O jogo, inspirado em Jet Fighter (1975), consiste em duas naves, uma vermelha e outra azul, que devem atirar uma na outra, sendo a vencedora aquela que acertar três tiros primeiro.

As naves possuem velocidade constante, sendo utilizadas as teclas "A", "D", e setas para esquerda e direita do teclado para redirecioná-las. Além disso, as teclas "W" e seta direcional para cima são usadas para efetuar disparos.

## Técnicas utilizadas
### Nave
Com relação à renderização das naves, foram criados quatro pontos, posteriormente ligados com a primitiva `GL_TRIANGLE_FAN`.
Após ser alvejada, a nave permanece invulneravel por um curto período de tempo, implementado a partir da utilização da classe `abcg::ElapsedTimer`. Durante este período, a imagem da nave deixa de ser renderizada, alternadamente, por alguns milissegundos.

### Projéteis
Cada nave pode disparar diversos projéteis, respeitando um tempo de recarga de 4 disparos por segundo. Os projéteis são disparados na direção da nave e viajam até colidirem com a borda da tela ou com a nave adversária.

### Vidas
As vidas de cada nave são representadas na forma de corações no topo da tela, desenhadas a partir de um conjunto de pontos utilizando a primitiva `GL_POINTS`. O tamanho de cada ponto foi alterado com a propriedade `gl_PointSize`, para obter melhor preenchimento do desenho.

### Mundo
O mundo é constituído por um espaço finito e estacionário. Ao sair pelas bordas, as naves reaparecem no outro lado, enquanto que as balas são destruídas.

### Controles
Foram utilizados os eventos da SDL para detectar as entradas do teclado que controlam as naves.

### UI
Para a interface gráfica, composta pela mensagem do vencedor e o botão de reiniciar, foi feita com a biblioteca *Dear ImGui*.
