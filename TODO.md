# Features

[x] Toupeira pode se mover por áreas soterradas
	
[x] Fog of war (9x9)

[ ] 3 vidas
    [x] Perder vida = reset inimigos vivos e jogador; apenas
    [x] Game over

[x] Pontos
    [x] Esmeralda - 100
    [x] Ouro - 50
    [x] Toupeira - 200

[x] Tiro
    [x] Único
    [x] Abre área soterrada

[x] Coletáveis
    [x] Ouro
        [x] pontos
    [x] Esmeralda
        [x] Coletar todas implica fim da fase
    [x] Power up
        [x] Visibilidade mapa inteiro (3s)
        
[x] HUD
    [x] Vidas
    [x] Pontuacao
    [x] Nivel
    [x] Esmeraldas_coletadas / Esmeraldas_totais
    
[ ] Mapas
    [ ] Nome dinâmico
    [ ] Mínimo 3 mapas
        [x] Mapa 1
        [x] Mapa 2
        [ ] Mapa 3
    [ ] Mas é dinâmico para qualquer numero de mapas
    [ ] Mínimo 30x20 (dimensões) cada um
    
[ ] Menu principal
    [x] (N) Novo jogo
    [ ] (C) Carregar jogo
    [x] (Q) Sair

[ ] Menu in-game
    [x] (N) Novo Jogo: Quando o usuário seleciona esta opção, um novo jogo é iniciado, a partir do
primeiro nível. Todo progresso e número de vidas do jogador também é resetado.
    [ ] (C) Carregar jogo: Quando o usuário seleciona esta opção, um jogo previamente salvo deve ser
carregado. Você pode assumir que existe apenas um jogo salvo e que pode ser carregado.
    [ ] (S) Salvar jogo: Quando o usuário seleciona esta opção, deve-se salvar todas as informações
pertinentes do jogo em um arquivo, de modo que ele possa ser carregado e continuado do ponto
em que foi salvo. Essas informações incluem posições do jogador e dos monstros, estados das
paredes, itens desbloqueados, número de vidas, fase, etc. Tudo que for necessário para o jogo
passar a funcionar normalmente a partir daquele ponto, como se o usuário nunca tivesse parado de
jogar.
    [x] (Q) Sair do jogo: Quando o usuário seleciona esta opção, o jogo é finalizado, sem salvar.
    [x] (V) Voltar: Quando o usuário seleciona esta opção, deve-se abandonar o menu e o controle volta
para o jogo, que continua do ponto em que parou quando o usuário acessou o menu.

[ ] Game over
    [ ] Casos
        [ ] Win
        [ ] Lose
    [ ] Mostrar pontos

---

# Problemas

[x] Bala deve ser apagada quando o jogador toma dano
