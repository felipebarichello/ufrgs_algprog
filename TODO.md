[x] Toupeira pode se mover por áreas soterradas
	
[x] Fog of war (9x9) - 

[ ] 3 vidas //Guto
    [x] Perder vida = reset inimigos vivos e jogador; apenas

[ ] Pontos //Guto
    Esmeralda - 100
    Ouro - 50
    Touperia - 200

[x] Tiro
    [x] Único
    [x] Abre área soterrada

[ ] Coletáveis //Guto
    [ ] Ouro
        pontos
    [ ] Esmeralda
        Coletar todas implica fim da fase
    [ ] Power up
        *Visibilidade mapa inteiro (3s)
    [ ] Life Up - maybe
        etc.
        
[ ] HUD //Guto
    Vidas
    Pontuacao
    Nivel
    Esmeraldas_coletadas / Esmeraldas_totais
    
[ ] Mapas //Guto
    [ ] Nome dinâmico
        mapa1.txt
        mapa2.txt
        mapa3.txt
        .
        .
        .
        mapan.txt
    [ ] Mínimo 3 mapas
        Mas deve ser dinâmico para qualquer numero de mapas
    [ ] Mínimo 30x20 (dimensões) cada um
    
[ ] Tela inicial
    [ ] Novo jogo
    [ ] Carregar jogo
    [ ] Sair

[ ] Game over
    [ ] Casos
        [ ] Win
        [ ] Lose
    [ ] Mostrar pontos
    
[ ] Menu  //Lipe
    [ ] (N) Novo Jogo: Quando o usuário seleciona esta opção, um novo jogo é iniciado, a partir do
primeiro nível. Todo progresso e número de vidas do jogador também é resetado.
    [ ] (C) Carregar jogo: Quando o usuário seleciona esta opção, um jogo previamente salvo deve ser
carregado. Você pode assumir que existe apenas um jogo salvo e que pode ser carregado.
    [ ] (S) Salvar jogo: Quando o usuário seleciona esta opção, deve-se salvar todas as informações
pertinentes do jogo em um arquivo, de modo que ele possa ser carregado e continuado do ponto
em que foi salvo. Essas informações incluem posições do jogador e dos monstros, estados das
paredes, itens desbloqueados, número de vidas, fase, etc. Tudo que for necessário para o jogo
passar a funcionar normalmente a partir daquele ponto, como se o usuário nunca tivesse parado de
jogar.
    [ ] (Q) Sair do jogo: Quando o usuário seleciona esta opção, o jogo é finalizado, sem salvar.
    [ ] (V) Voltar: Quando o usuário seleciona esta opção, deve-se abandonar o menu e o controle volta
para o jogo, que continua do ponto em que parou quando o usuário acessou o menu.