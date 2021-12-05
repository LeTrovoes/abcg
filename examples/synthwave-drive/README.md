# Synthwave

## Sobre
Uma aplicação gráfica 3D desenvolvida em C++, utilizando OpenGL juntamente com a biblioteca ABCg, para a disciplina de Computação Gráfica. Turma A1, diurno (DA1MCTA008-17SA), 3º quadrimestre de 2021.

A aplicação trata-se de uma demonstração de utilização das linguagens e bibliotecas citadas acima para exibição de imagens virtuais tridimensionais em movimento perpétuo. O projeto é inspirado na estética apresentada em clipes de músicas synthwave, caracterizados pelo movimento em direção ao por do sol, com esta paleta de cores característica apresentada.

## Técnicas utilizadas
###  Chão
O chão é disposto no plano xz. Seu efeito de movimento contínuo foi implementado a partir de duas etapas. Primeiramente, são desenhadas linhas horizontais e verticais no plano, em perspectiva. Posteriormente, as linhas horizontais são transladadas na direção da câmera até se deslocarem o equivalente à distância entre elas, momento em que a translação é reiniciada, sem ser percebida.

### Estrelas
Para reproduzir o efeito de céu estrelado, são dispostas circunferências aleatoriamente em um plano mais distante que os outros objetos, paralelo ao xy. As estrelas alteram sua opacidade continuamente e, ao atingir zero, é movida para outra posição aleatória do plano.

### Coelhos
Foi utilizado um modelo geométrico 3D de um coelho, sendo 2 deles dispostos nos lados da tela. Cada um dos objetos se mantem constantemente em rotação em torno do proprio eixo y, mantendo movimento espelhado em relação ao outro modelo. Seu sombreamento é feito alterando a cor dos fragmentos, com base na posição no eixo z.

### Gradiente
O gradiente rosa ao fundo foi feito com um plano, paralelo ao xy, posicionado ao fundo da cena, logo em frente ao sol e às estrelas. O degradê foi obtido por meio da funcionalidade `GL_BLEND`, variando a opacidade de 1 nos vértices de baixo para 0 nos de cima. Como consequência, ele também é responsável pela alteração da cor do sol.

### Transformações afins
Para todos os elementos acima, foram utilizadas transformações afins, responsáveis pelo posicionamento, rotação e dimensionamento dos objetos. Além disso, foram utilizadas transformações para a câmera (look-at) e projeção perspectiva.
