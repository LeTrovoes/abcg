# Synthwave Drive

## Sobre
Uma aplicação gráfica 3D desenvolvida em C++, utilizando OpenGL juntamente com a biblioteca ABCg, para a disciplina de Computação Gráfica. Turma A1, diurno (DA1MCTA008-17SA), 3º quadrimestre de 2021.

A aplicação trata-se de uma demonstração de utilização das linguagens e bibliotecas citadas acima para exibição de imagens virtuais tridimensionais em movimento perpétuo. O projeto é inspirado na estética apresentada em clipes de músicas synthwave, caracterizados pelo movimento em direção ao por do sol, com esta paleta de cores característica apresentada.

## Técnicas utilizadas
###  Chão
O chão é disposto no plano xz. Seu efeito de movimento contínuo foi implementado a partir de duas etapas. Primeiramente, são desenhadas linhas horizontais e verticais no plano, em perspectiva. Posteriormente, as linhas horizontais são transladadas na direção da câmera até se deslocarem o equivalente à distância entre elas, momento em que a translação é reiniciada, sem ser percebida.

### Brilho do chão
Para implementar o brilho da malha do chão, foi utilizado uma técnica de pós-processamento, semelhante ao Blur Gaussiano [1], porém com parâmetros alterados. O chão é desenhado em um _framebuffer object_, que possui uma textura 2D _multisample_ como anexo. A textura gerada é então transferida para um _framebuffer_ intermediário sem multisample, por meio da função `glBlitFramebuffer`, para que possa ser utilizada no fragment shader normalmente. Essa textura intermediária é então desenhada no primeiro FBO, de forma a preencher toda a tela, mas utilizando o fragment shader com o filtro. Após isso, é feito novamente o passo intermediário. Por fim, esta última textura gerada é desenhada, da mesma forma, mas dessa vez sobre a tela final, fazendo com que o filtro seja aplicado pela segunda vez.

### Estrelas
Para reproduzir o efeito de céu estrelado, são dispostas circunferências aleatoriamente em um plano mais distante que os outros objetos, paralelo ao xy. As estrelas alteram sua opacidade continuamente e, ao atingir zero, é movida para outra posição aleatória do plano.

### _DeLorean™_
Foi utilizado o modelo de um carro _DMC Delorean™_, juntamente com sua textura, obtidos em [2]. O carro é capaz de se movimentar pelo eixo x da tela por meio das setas direcionais, ou teclas A e D, do teclado. O modelo é iluminado por uma luz rosa à sua frente e em cima.
### Palmeiras
As palmeiras são dispostas em sequência, em ambos os lados da cena. Elas são iluminadas por meio do algoritmo Blinn-Phong, tendo cada um dos objetos sua propria fonte de luz direcional, posicionadas na parte central e anterior da tela, representando a luz do sol posicionado ao fundo. O modelo utilizado pode ser encontrado em [3].

### Gradiente
O gradiente rosa ao fundo foi feito com um plano, paralelo ao xy, posicionado ao fundo da cena, logo em frente ao sol e às estrelas. O degradê foi obtido por meio da funcionalidade `GL_BLEND`, variando a opacidade de 1 nos vértices de baixo para 0 nos de cima. Como consequência, ele também é responsável pela alteração da cor do sol.

### Transformações afins
Para todos os elementos acima, foram utilizadas transformações afins, responsáveis pelo posicionamento, rotação e dimensionamento dos objetos. Além disso, foram utilizadas transformações para a câmera (look-at) e projeção perspectiva.

### Fontes
[[1]](https://github.com/Jam3/glsl-fast-gaussian-blur) - Algoritmo Blur Gaussiano

[[2]](https://www.cgtrader.com/free-3d-models/car/racing/delorean-cb42ca60-6300-48c9-826f-c6be0de634e1) - Modelo _DeLorean™_

[[3]](https://www.cadnav.com/3d-models/model-29995.html) - Modelo Palmeiras
