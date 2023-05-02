Sim, é possível usar a biblioteca Box2D para realizar cálculos
sem precisar renderizar imagens. Neste caso, você pode criar um
mundo de física, adicionar os corpos e fixações desejados, e
então chamar a função Step() para atualizar a simulação a cada
passo de tempo.

Segue abaixo um exemplo simples de como usar a biblioteca Box2D
para simular um lançamento: Este exemplo cria um mundo de
física, adiciona um corpo de projétil com uma fixação circular,
define sua posição e velocidade inicial, e então simula seu
lançamento por 10 segundos. A cada passo de tempo, a posição
atual do projétil é impressa na tela.
