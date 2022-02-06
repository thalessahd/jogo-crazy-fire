#include "PIG.h"
#include <cstdlib>
#include <ctime>

PIG_Evento evento;          //evento ser tratado a cada passada do loop principal
PIG_Teclado meuTeclado;     //variável como mapeamento do teclado

int map_height = 720;
// Tamanho do mapa = 960x720 | Tamanho da tela: 960x770

/*
Desenha os retangulos para testes.
O jogador não pode passar dos retangulos em azul (há colisão)
Os retangulos em verde representam as entradas de onde saem os inimigos, o jogador pode andar nessa área
Estes retangulos NÃO estarão presentes na versão final do jogo, servem apenas para fins de testes enquanto o modelo final do mapa não foi implementado.
*/

void test_draw(){
    DesenhaRetangulo(0,0,40,PIG_LARG_TELA,AZUL);
    DesenhaRetangulo(0,0,map_height,40,AZUL);
    DesenhaRetangulo(PIG_LARG_TELA-40,0,map_height,40,AZUL);
    DesenhaRetangulo(0,map_height-40,40,PIG_LARG_TELA,AZUL);
    DesenhaRetangulo((PIG_LARG_TELA/2)-60,0,40,120,VERDE);
    DesenhaRetangulo(0,(map_height/2)-60,120,40,VERDE);
    DesenhaRetangulo((PIG_LARG_TELA/2)-60,map_height-40,40,120,VERDE);
    DesenhaRetangulo(PIG_LARG_TELA-40,(map_height/2)-60,120,40,VERDE);
}

//Desenha a tela inicial do jogo
void draw_inicial(int aux){

    DesenhaSpriteSimples("..//images//tela_inicial.png",0,0,0);
    EscreverCentralizada("(1) - NOVO JOGO",PIG_LARG_TELA/2,PIG_ALT_TELA-650,BRANCO);
    EscreverCentralizada("(2) - SAIR",PIG_LARG_TELA/2,PIG_ALT_TELA-700,BRANCO);
    if(aux == 1){
        EscreverCentralizada("(1) - NOVO JOGO",PIG_LARG_TELA/2,PIG_ALT_TELA-650,VERDE);
    }else{
        EscreverCentralizada("(2) - SAIR",PIG_LARG_TELA/2,PIG_ALT_TELA-700,VERDE);
    }
}

//Desenha as telas de dialogo
void draw_dialogo(int stage){
    if(stage == 1){
        DesenhaSpriteSimples("..//images//intro1.png",0,0,0);
    }else if(stage == 2){
        DesenhaSpriteSimples("..//images//intro2.png",0,0,0);
    }else if(stage == 3){
        DesenhaSpriteSimples("..//images//intro3.png",0,0,0);
    }else if(stage == 4){
        DesenhaSpriteSimples("..//images//intro4.png",0,0,0);
    }else if(stage == 5){
        DesenhaSpriteSimples("..//images//intro5.png",0,0,0);
    }else if(stage == 6){
        DesenhaSpriteSimples("..//images//intro6.png",0,0,0);
    }else if(stage == 7){
        DesenhaSpriteSimples("..//images//intro7.png",0,0,0);
    }else if(stage == 8){
        DesenhaSpriteSimples("..//images//intro8.png",0,0,0);
    }
}

//Desenha todos os elementos da hud
void hud_draw(int char_id, int hp, int extinguisher_id, int *ext, int *charge){
    int i =0;
    int posx[4] = {550,660,770,880}; //Posição x de cada numero de munição dos extintores

    DesenhaObjeto(char_id); //Desenha a imagem do personagem principal na HUD
    DesenhaRetangulo(60,PIG_ALT_TELA-40,30,hp,VERMELHO); //Desenha a barra de vida do personagem na HUD

    //Desenha extintores na HUD
    for(i=0;i<4;i++){
        DesenhaObjeto(ext[i]);
        SetColoracaoObjeto(ext[i],BRANCO);
    }

    /*
    Escrita da "munição" na tela
    Numero em branco caso a munição estiver acima da metade
    Em amarelo se estiver entre 25% e 50%
    Em vermelho nos ultimos 25%
    */
    for(i=0;i<4;i++){
        if(charge[i] < 25){
            EscreveInteiroEsquerda(charge[i],posx[i],PIG_ALT_TELA-50,VERMELHO);
        }else if(charge[i] >= 25 && charge[i] <= 50){
            EscreveInteiroEsquerda(charge[i],posx[i],PIG_ALT_TELA-50,AMARELO);
        }else{
            EscreveInteiroEsquerda(charge[i],posx[i],PIG_ALT_TELA-50,BRANCO);
        }
    }

    //Destaca o extintor selecionado
    //SetColoracaoObjeto(ext[extinguisher_id-1],VERDE);
    if(extinguisher_id == 1){
        DesenhaRetanguloVazado(505,PIG_ALT_TELA-50,48,44,VERDE);
    }else if(extinguisher_id == 2){
        DesenhaRetanguloVazado(615,PIG_ALT_TELA-50,48,44,VERDE);
    }else if(extinguisher_id == 3){
        DesenhaRetanguloVazado(725,PIG_ALT_TELA-50,48,44,VERDE);
    }else if(extinguisher_id == 4){
        DesenhaRetanguloVazado(835,PIG_ALT_TELA-50,48,44,VERDE);
    }
}

//Mensagem de game over
void gameover(){
    //EscreverCentralizada("GAME OVER",PIG_LARG_TELA/2,map_height/2,VERMELHO);
    DesenhaSpriteSimples("..//images//game-over.png",0,0,0);
}

//Mensagem de pause
void gamepaused(){
    EscreverCentralizada("PAUSADO",PIG_LARG_TELA/2,map_height/2,AMARELO);
}

//Inicia os timers controladores após o fim do tutorial
void starttimers(int t1, int t2, int t3, int t4, int t5, int t6, int t7, int t8){
    ReiniciaTimer(t1);
    ReiniciaTimer(t2);
    ReiniciaTimer(t3);
    ReiniciaTimer(t4);
    ReiniciaTimer(t5);
    ReiniciaTimer(t6);
    ReiniciaTimer(t7);
    ReiniciaTimer(t8);
}

//Escreve a escolha de dificuldade na tela
void draw_nivel(int nivel_aux){
    EscreverCentralizada("ESCOLHA A DIFICULDADE",PIG_LARG_TELA/2,PIG_ALT_TELA-100,VERMELHO);
    EscreverCentralizada("(1) - FACIL",PIG_LARG_TELA/2,PIG_ALT_TELA-200,BRANCO);
    EscreverCentralizada("(2) - NORMAL",PIG_LARG_TELA/2,PIG_ALT_TELA-300,BRANCO);
    EscreverCentralizada("(3) - DIFICIL",PIG_LARG_TELA/2,PIG_ALT_TELA-400,BRANCO);
    EscreverCentralizada("(4) - IMPOSSIVEL",PIG_LARG_TELA/2,PIG_ALT_TELA-500,BRANCO);
    if(nivel_aux == 1){
        EscreverCentralizada("(1) - FACIL",PIG_LARG_TELA/2,PIG_ALT_TELA-200,VERDE);
    }else if(nivel_aux == 2){
        EscreverCentralizada("(2) - NORMAL",PIG_LARG_TELA/2,PIG_ALT_TELA-300,VERDE);
    }else if(nivel_aux == 3){
        EscreverCentralizada("(3) - DIFICIL",PIG_LARG_TELA/2,PIG_ALT_TELA-400,VERDE);
    }else if(nivel_aux == 4){
        EscreverCentralizada("(4) - IMPOSSIVEL",PIG_LARG_TELA/2,PIG_ALT_TELA-500,VERDE);
    }
}

//Escreve o tutorial na tela
void draw_tutorial(){
    /*EscreverCentralizada("TUTORIAL",PIG_LARG_TELA/2,PIG_ALT_TELA-100,VERMELHO);
    EscreverEsquerda("(1) - Extintor de Espuma -> Classes A e B",50,600,BRANCO);
    EscreverEsquerda("(2) - Extintor de Dioxido de Carbono -> Classes B e C",50,500,BRANCO);
    EscreverEsquerda("(3) - Extintor Quimico -> Classes B, C e D",50,400,BRANCO);
    EscreverEsquerda("(4) - Extintor de Agua -> Classe A",50,300,BRANCO);
    EscreverEsquerda("(5) - Extintor Especial -> Todas as Classes",50,200,BRANCO);
    EscreverCentralizada("APERTE ENTER PARA INICIAR",PIG_LARG_TELA/2,100,VERDE);*/
    DesenhaSpriteSimples("..//images//tutorial.png",0,0,0);
}

//Mensagem de vitória
void game_win(){
    //EscreverCentralizada("VITORIA",PIG_LARG_TELA/2,map_height/2,VERDE);
    DesenhaSpriteSimples("..//images//vitoria.png",0,0,0);
}

int main( int argc, char* args[] ){

    CriaJogo("Crazy Fire");

    srand((unsigned int)time(NULL));

    /*
    Variaveis do personagem principal e inimigos:

    x_main_char e y_main_char           = Posição atual do personagem
    x_enemy e y_enemy                   = Posição atual dos inimigos
    kills                               = Contagem de quantas chamas foram apagadas
    kills_to_win                        = Quantas chamas precisam ser apagadas para o jogador vencer
    main_char_length e main_char_height = Dimensões do personagem principal
    hp                                  = Vida atual do personagem (O jogador perde caso chegue a 0)
    ext_id                              = Representa o extintor selecionado atualmente
    spawn_id                            = Em qual dos 4 locais de spawn que a chama irá aparecer
    nivel                               = Qual é a dificuldade do jogo
    nivel_aux                           = Auxiliar a escolha da dificuldade
    inicial_aux                         = Auxiliar a escolha do menu inicial
    dialogo_stage                       = Qual parte do dialogo será mostrada
    max_hp                              = Maximo de vida que o personagem pode ter
    heal                                = Quanto que um coração cura o personagem

    */

    int x_main_char = 0, y_main_char = 0, i = 0, x_enemy = 0, y_enemy = 0, kills = 0, kills_to_win = 10, dialogo_stage = 1, max_hp = 400, heal = 50;
    int main_char_length = 40, main_char_height = 40, hp = 400, ext_id = 1, spawn_id = 0, nivel = 0, nivel_aux = 1, inicial_aux = 1;

    /*
    Vetores

    charge       = Munição atual de cada um dos extintores
    enemy        = Guarda o ID de cada um dos inimigos
    enemy_id     = Guarda o TIPO de chama que o inimigo representa
    extinguisher = Guarda o ID de cada extintor para a HUD
    invis_wall   = Guarda o ID de cada parede invisivel que serve para delimitar o mapa

    */

    int charge[4] = {100,100,100,100}, enemy[10] = {0,0,0,0,0,0,0,0,0,0}, enemy_id[10] = {0,0,0,0,0,0,0,0,0,0}, extinguisher[4] = {0,0,0,0}, invis_wall[8] = {0,0,0,0,0,0,0,0};

    /*
    Controladores de timers

    move_check_time   = Quantos segundos para o personagem poder se movimentar
    empty_ext_time    =    ||      ||     ||  o extintor perder 1 de "munição"
    reload_spawn_time =    ||      ||     ||  a recarga aparecer no mapa
    enemy_spawn_time  =    ||      ||     ||  uma chama aparecer
    enemy_move_time   =    ||      ||     ||  uma chama poder se movimentar
    damage_taken_time =    ||      ||     ||  o personagem poder receber dano
    hp_spawn_time     =    ||      ||     ||  o coração de hp aparecer
    speed_spawn_time  =    ||      ||     ||  a bota de velocidade aparecer

    */

    float move_check_time = 0.004, empty_ext_time = 0.01, reload_spawn_time = 5, enemy_spawn_time = 3, enemy_move_time = 0.008, damage_taken_time = 0.025, hp_spawn_time = 7, speed_spawn_time = 9;

    /*
    Flags

    draw_smoke  = Se a fumaça do extintor deve ser desenhada na tela
    draw_reload = Se o extintor de recarga deve ser desenhado na tela
    game_over   = Se o jogo acabou com uma derrota
    win         = Se o jogo acabou com uma vitória
    final_msg   = Se a mensagem final ja foi exibida
    tutorial    = Se o tutorial ja foi exibido
    draw_enemy  = Se e quais inimigos devem ser desenhados na tela
    paused      = Se o jogo está pausado ou não
    level       = Se a tela de nivel precisa ser desenhada
    set_var     = Setar variaveis de acordo com a dificuldade escolhida
    inicial     = Se a tela inicial deve ser exibida
    draw_heart  = Se o coração de hp deve ser desenhado na tela
    draw_speed  = Se a bota de velocidade deve ser desenhada na tela

    */

    bool draw_smoke = false, draw_reload = false, game_over = false, final_msg = false, tutorial = false, win = false, paused = false, level = false, set_var = false, inicial = true, dialogo = false;
    bool draw_enemy[10] = {false,false,false,false,false,false,false,false,false,false}, draw_speed = false, draw_heart = false;

    meuTeclado = GetTeclado();

    int timer               = CriaTimer(1); //Para checar movimento
    int timershot           = CriaTimer(1); //Para checar disparo do extintor
    int timer_reload        = CriaTimer(1); //Para checar spawn do extintor de recarga
    int timer_enemy         = CriaTimer(1); //Para checar spawn dos inimigos
    int timer_enemy_move    = CriaTimer(1); //Para checar movimento do inimigo
    int timer_damage        = CriaTimer(1); //Para checar dano sofrido
    int timer_hp            = CriaTimer(1); //Para checar spawn de vida
    int timer_speed         = CriaTimer(1); //Para checar spawn de velocidade

    //Criando imagens dos extintores para a HUD
    extinguisher[0] = CriaObjeto("..//images//extinguisher1.png");
    extinguisher[1] = CriaObjeto("..//images//extinguisher2.png");
    extinguisher[2] = CriaObjeto("..//images//extinguisher3.png");
    extinguisher[3] = CriaObjeto("..//images//extinguisher4.png");
    for(i=0;i<4;i++){
        //extinguisher[i] = CriaObjeto("..//images//extinguisher.png");
        SetDimensoesObjeto(extinguisher[i],50,50);
        MoveObjeto(extinguisher[i],500+110*i,PIG_ALT_TELA-50);
    }

    //Criando power-up de velocidade
    int speed = CriaObjeto("..//images//poderBota.png");
    SetDimensoesObjeto(speed,40,40);

    //Criando coração de hp
    int heart = CriaObjeto("..//images//heart.png");
    SetDimensoesObjeto(heart,40,40);

    //Criando extintor de recarga
    int reload = CriaObjeto("..//images//extinguisher0.png");
    SetDimensoesObjeto(reload,40,40);

    //Criando imagem do personagem para a HUD
    int hudchar = CriaObjeto("..//images//char_front.png",1);
    SetDimensoesObjeto(hudchar,50,50);
    MoveObjeto(hudchar,0,PIG_ALT_TELA-50);

    //Criando fumaça do extintor
    int smoke = CriaObjeto("..//images//smoke.png");
    SetDimensoesObjeto(smoke,50,50);

    //Gerando o sprite do personagem principal e aplicando suas dimensões
    int main_char = CriaObjeto("..//images//main_char.png",1);
    CarregaArquivoFramesObjeto(main_char,"..//frames//main_char.txt");
    SetDimensoesObjeto(main_char,main_char_height,main_char_length);
    MoveObjeto(main_char,PIG_LARG_TELA/2,map_height/2);
    MudaFrameObjeto(main_char,1);

    //Criando paredes de colisão
    for(i=0;i<8;i++){
        invis_wall[i] = CriaObjeto("..//images//branco.png",0);
        if(i<=3){
            SetDimensoesObjeto(invis_wall[i],40,(PIG_LARG_TELA/2)-60);
        }else{
            SetDimensoesObjeto(invis_wall[i],(map_height/2)-60,40);
        }
    }

    //Movendo paredes de colisão para seus respectivos lugares
    MoveObjeto(invis_wall[0],0,0); //bot
    MoveObjeto(invis_wall[1],(PIG_LARG_TELA/2)+60,0); //bot2
    MoveObjeto(invis_wall[2],0,map_height-40); //top
    MoveObjeto(invis_wall[3],(PIG_LARG_TELA/2)+60,map_height-40); //top2
    MoveObjeto(invis_wall[4],0,(map_height/2)+60); //left2
    MoveObjeto(invis_wall[5],PIG_LARG_TELA-40,0); //right
    MoveObjeto(invis_wall[6],PIG_LARG_TELA-40,(map_height/2)+60); //right2

    while(JogoRodando()){

        evento = GetEvento();

        GetXYObjeto(main_char,&x_main_char,&y_main_char); //Variaveis que guardam a posição atual do personagem principal

        //Checa por um input de teclado a cada X segundos e não movimenta o personagem caso ele se encontre em colisão com o mapa | muda o sprite do personagem de acordo com a direção do movimento
        if(TempoDecorrido(timer) > move_check_time){
            if(y_main_char < map_height-40 && (meuTeclado[PIG_TECLA_CIMA] || meuTeclado[PIG_TECLA_w]) && (!TestaColisaoObjetos(main_char,invis_wall[2]) && !TestaColisaoObjetos(main_char,invis_wall[3]) && (!TestaColisaoObjetos(main_char,invis_wall[4]) || x_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall[6]) || x_main_char <=PIG_LARG_TELA-79))){
                DeslocaObjeto(main_char,0,+1);
                MudaFrameObjeto(main_char,2);
            }
            if(y_main_char > 0 && (meuTeclado[PIG_TECLA_BAIXO] || meuTeclado[PIG_TECLA_s]) && (!TestaColisaoObjetos(main_char,invis_wall[0]) && !TestaColisaoObjetos(main_char,invis_wall[1]) && (!TestaColisaoObjetos(main_char,invis_wall[7]) || x_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall[5]) || x_main_char <=PIG_LARG_TELA-79))){
                DeslocaObjeto(main_char,0,-1);
                MudaFrameObjeto(main_char,1);
            }
            if(x_main_char > 0 && (meuTeclado[PIG_TECLA_ESQUERDA] || meuTeclado[PIG_TECLA_a]) && (!TestaColisaoObjetos(main_char,invis_wall[7]) && !TestaColisaoObjetos(main_char,invis_wall[4]) && (!TestaColisaoObjetos(main_char,invis_wall[0]) || y_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall[2]) || y_main_char <= map_height-79))){
                DeslocaObjeto(main_char,-1,0);
                MudaFrameObjeto(main_char,4);
            }
            if(x_main_char < PIG_LARG_TELA-40 && (meuTeclado[PIG_TECLA_DIREITA] || meuTeclado[PIG_TECLA_d]) && (!TestaColisaoObjetos(main_char,invis_wall[5]) && !TestaColisaoObjetos(main_char,invis_wall[6]) && (!TestaColisaoObjetos(main_char,invis_wall[1]) || y_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall[3]) || y_main_char <= map_height-79))){
                DeslocaObjeto(main_char,+1,0);
                MudaFrameObjeto(main_char,3);
            }
            ReiniciaTimer(timer);
        }

        //Setando variáveis de acordo com a dificuldade escolhida
        if(set_var){
            if(nivel == 1){
                hp = 400;
                //kills_to_win = 10;
                empty_ext_time = 0.01;
                enemy_move_time = 0.008;
                damage_taken_time = 0.025;
            }else if(nivel == 2){
                hp = 350;
                //kills_to_win = 15;
                empty_ext_time = 0.009;
                enemy_move_time = 0.007;
                damage_taken_time = 0.023;
            }else if(nivel == 3){
                hp = 300;
                //kills_to_win = 20;
                empty_ext_time = 0.008;
                enemy_move_time = 0.006;
                damage_taken_time = 0.021;
            }else if(nivel == 4){
                hp = 250;
                //kills_to_win = 25;
                empty_ext_time = 0.007;
                enemy_move_time = 0.005;
                damage_taken_time = 0.019;
            }
            set_var = false;
        }


        //Checa seleção de extintor
        if(meuTeclado[PIG_TECLA_1]){
            ext_id = 1;
        }
        if(meuTeclado[PIG_TECLA_2]){
            ext_id = 2;
        }
        if(meuTeclado[PIG_TECLA_3]){
            ext_id = 3;
        }
        if(meuTeclado[PIG_TECLA_4]){
            ext_id = 4;
        }
        //if(meuTeclado[PIG_TECLA_5]){
           // ext_id = 5;
        //}

        //Checa disparo do extintor e desenha a fumaça de acordo com a direção do personagem. Diminui "munição" do extintor de acordo com o timer e a variavel de controle.
        if(meuTeclado[PIG_TECLA_ENTER]){
            if(charge[ext_id-1] > 0){
                if(GetFrameAtualObjeto(main_char)==1){
                    MoveObjeto(smoke, x_main_char, y_main_char - main_char_height);
                }else if(GetFrameAtualObjeto(main_char)==2){
                    MoveObjeto(smoke, x_main_char, y_main_char + main_char_height);
                }else if(GetFrameAtualObjeto(main_char)==3){
                    MoveObjeto(smoke, x_main_char + main_char_length, y_main_char);
                }else if(GetFrameAtualObjeto(main_char)==4){
                    MoveObjeto(smoke, x_main_char - main_char_length, y_main_char);
                }
                if(TempoDecorrido(timershot) > empty_ext_time){
                    charge[ext_id-1]--;
                    ReiniciaTimer(timershot);
                }
                draw_smoke = true;
            }
        }
        if(!draw_smoke){
            MoveObjeto(smoke,0,PIG_ALT_TELA);
        }

        //Spawn do extintor de recarga no mapa
        if(TempoDecorrido(timer_reload) > reload_spawn_time){
            MoveObjeto(reload,rand()%800+50,rand()%600+50);
            draw_reload = true;
            ReiniciaTimer(timer_reload);
        }

        //Spawn do coração de cura no mapa
        if(TempoDecorrido(timer_hp) > hp_spawn_time){
            MoveObjeto(heart,rand()%800+50,rand()%600+50);
            draw_heart = true;
            ReiniciaTimer(timer_hp);
        }

        //Spawn da bota de velocidade no mapa
        if(TempoDecorrido(timer_speed) > speed_spawn_time){
            MoveObjeto(speed,rand()%800+50,rand()%600+50);
            draw_speed = true;
            ReiniciaTimer(timer_speed);
        }

        //Fecha o jogo apertando ESC
        if(meuTeclado[PIG_TECLA_ESC]){
            break;
        }

        //Pegar recarga de extintor no mapa
        if(TestaColisaoObjetos(main_char,reload) && draw_reload){
            for(i=0;i<4;i++){
                if(charge[i]>=75){
                    charge[i] = 100;
                }else{
                    charge[i] = charge[i] + 25;
                }
            }
            draw_reload = false;
            ReiniciaTimer(timer_reload);
        }

        //Pegar coração de cura no mapa
        if(TestaColisaoObjetos(main_char,heart) && draw_heart){
            hp = hp + heal;
            if(hp > max_hp){
                hp = max_hp;
            }
            draw_heart = false;
            ReiniciaTimer(timer_hp);
        }

        //Pegar bota de velocidade no mapa
        if(TestaColisaoObjetos(main_char,speed) && draw_speed){
            move_check_time = 0.002;
            draw_speed = false;
            ReiniciaTimer(timer_speed);
            PausaTimer(timer_speed);
        }

        //Criação e spawn de inimigos
        if(TempoDecorrido(timer_enemy) > enemy_spawn_time){
            spawn_id = rand()%4;

            for(i=0;i<10;i++){
                if(!draw_enemy[i]){
                    enemy_id[i] = rand()%4;
                    if(enemy_id[i] == 0){
                        enemy[i] = CriaObjeto("..//images//flameA.png");
                    }else if(enemy_id[i] == 1){
                        enemy[i] = CriaObjeto("..//images//flameB.png");
                    }else if(enemy_id[i] == 2){
                        enemy[i] = CriaObjeto("..//images//flameC.png");
                    }else if(enemy_id[i] == 3){
                        enemy[i] = CriaObjeto("..//images//flameD.png");
                    }
                    SetDimensoesObjeto(enemy[i],50,50);
                    if(spawn_id == 0){
                        MoveObjeto(enemy[i],(PIG_LARG_TELA/2),map_height-30);
                    }else if(spawn_id == 1){
                        MoveObjeto(enemy[i],PIG_LARG_TELA,(map_height)/2);
                    }else if(spawn_id == 2){
                        MoveObjeto(enemy[i],(PIG_LARG_TELA/2),-50);
                    }else if(spawn_id == 3){
                        MoveObjeto(enemy[i],-50,(map_height)/2);
                    }
                    draw_enemy[i] = true;
                    break;
                }
            }
            ReiniciaTimer(timer_enemy);
        }

        //Movimentação dos inimigos
        if(TempoDecorrido(timer_enemy_move) > enemy_move_time){
            for(i=0;i<10;i++){
                if(draw_enemy[i]){
                    GetXYObjeto(enemy[i],&x_enemy,&y_enemy);
                if(x_enemy < x_main_char && x_enemy < PIG_LARG_TELA-85){
                    x_enemy++;
                }else if(x_enemy > x_main_char && x_enemy > 35){
                    x_enemy--;
                }
                if(y_enemy < y_main_char && y_enemy < map_height-85){
                    y_enemy++;
                }else if(y_enemy > y_main_char && y_enemy > 35){
                    y_enemy--;
                }
                MoveObjeto(enemy[i],x_enemy,y_enemy);
                }
            }
            ReiniciaTimer(timer_enemy_move);
        }

        //Mecanicas de dano sofrido
        SetColoracaoObjeto(main_char,BRANCO);
        for(i=0;i<10;i++){
            if(TestaColisaoObjetos(main_char,enemy[i])){
                SetColoracaoObjeto(main_char,VERMELHO);
                if(TempoDecorrido(timer_damage) > damage_taken_time){
                    hp--;
                    ReiniciaTimer(timer_damage);
                }
            }
        }

        //Colisão entre o disparo e os inimigos
        for(i=0;i<10;i++){
            if(draw_enemy[i]){
                if(TestaColisaoObjetos(enemy[i],smoke)){
                    if(ext_id == 1 && (enemy_id[i] == 0 || enemy_id[i] == 1)){
                        draw_enemy[i] = false;
                        MoveObjeto(enemy[i],0,PIG_ALT_TELA);
                        //DestroiObjeto(enemy[i]);
                        kills++;
                    }
                    if(ext_id == 2 && (enemy_id[i] == 1 || enemy_id[i] == 2)){
                        draw_enemy[i] = false;
                        MoveObjeto(enemy[i],0,PIG_ALT_TELA);
                        //DestroiObjeto(enemy[i]);
                        kills++;
                    }
                    if(ext_id == 3 && (enemy_id[i] == 1 || enemy_id[i] == 2 || enemy_id[i] == 3)){
                        draw_enemy[i] = false;
                        MoveObjeto(enemy[i],0,PIG_ALT_TELA);
                        //DestroiObjeto(enemy[i]);
                        kills++;
                    }
                    if(ext_id == 4 && enemy_id[i] == 0){
                        draw_enemy[i] = false;
                        MoveObjeto(enemy[i],0,PIG_ALT_TELA);
                        //DestroiObjeto(enemy[i]);
                        kills++;
                    }
                }
            }
        }

        //Condição de vitória
        if(kills >= kills_to_win){
            win = true;
        }

        //Condição de game over
        if(hp <= 0){
            game_over = true;
        }

        //Responsável pelo pause e unpause do jogo (Crashando no unpause)
        while(paused){
            if(meuTeclado[PIG_TECLA_u]){
                paused = false;
                DespausaTudo();
                break;
            }
        }
        if(meuTeclado[PIG_TECLA_p]){
            paused = true;
            PausaTudo();
        }


        IniciaDesenho();

        //Desenha o tutorial na tela
        if(tutorial){
            draw_tutorial();
            if(evento.tipoEvento == PIG_EVENTO_TECLADO && evento.teclado.acao == PIG_TECLA_LIBERADA && evento.teclado.tecla == PIG_TECLA_ENTER){
                tutorial = false;
                starttimers(timer,timershot,timer_damage,timer_enemy,timer_enemy_move,timer_reload,timer_hp,timer_speed);
            }
        }

        //Desenha as telas de dialogo
        if(dialogo){
            draw_dialogo(dialogo_stage);
            if(evento.tipoEvento == PIG_EVENTO_TECLADO && evento.teclado.acao == PIG_TECLA_LIBERADA && evento.teclado.tecla == PIG_TECLA_ENTER){
                dialogo_stage++;
                if(dialogo_stage >= 8){
                    tutorial = true;
                    dialogo = false;
                }
            }
        }

        //Desenha a tela de escolha da dificuldade
        if(level){
            draw_nivel(nivel_aux);
            if(meuTeclado[PIG_TECLA_1]){
                nivel_aux = 1;
            }else if(meuTeclado[PIG_TECLA_2]){
                nivel_aux = 2;
            }else if(meuTeclado[PIG_TECLA_3]){
                nivel_aux = 3;
            }else if(meuTeclado[PIG_TECLA_4]){
                nivel_aux = 4;
            }
            if(evento.tipoEvento == PIG_EVENTO_TECLADO && evento.teclado.acao == PIG_TECLA_LIBERADA && evento.teclado.tecla == PIG_TECLA_ENTER){
                nivel = nivel_aux;
                set_var = true;
                level = false;
                dialogo = true;
            }
        }

        //Desenha a tela inicial do jogo
        if(inicial){
            draw_inicial(inicial_aux);
            if(meuTeclado[PIG_TECLA_1]){
                inicial_aux = 1;
            }else if(meuTeclado[PIG_TECLA_2]){
                inicial_aux = 2;
            }
            if(evento.tipoEvento == PIG_EVENTO_TECLADO && evento.teclado.acao == PIG_TECLA_LIBERADA && evento.teclado.tecla == PIG_TECLA_ENTER){
                if(inicial_aux == 1){
                    inicial = false;
                    level = true;
                }else{
                    return 0;
                }
            }
        }

        if(!tutorial && !level && !inicial && !dialogo){

            //Desenha o mapa
            DesenhaSpriteSimples("..//images//map_holder.png",0,0,0);

            //Desenha HUD e retangulos para teste
            //test_draw();
            hud_draw(hudchar,hp,ext_id,extinguisher,charge);

            //Desenha personagem principal
            DesenhaObjeto(main_char);

            //Desenha fumaça do extintor, extintor de recarga, coração de cura e bota de velocidade
            if(draw_smoke){
                DesenhaObjeto(smoke);
                draw_smoke = false;
            }
            if(draw_reload){
                DesenhaObjeto(reload);
            }
            if(draw_heart){
                DesenhaObjeto(heart);
            }
            if(draw_speed){
                DesenhaObjeto(speed);
            }

            //Desenha inimigos
            for(i=0;i<10;i++){
                if(draw_enemy[i]){
                    DesenhaObjeto(enemy[i]);
                    GetXYObjeto(enemy[i],&x_enemy,&y_enemy);
                    /*switch(enemy_id[i]){
                        case 0: EscreverCentralizada("A",x_enemy+25,y_enemy);break;
                        case 1: EscreverCentralizada("B",x_enemy+25,y_enemy);break;
                        case 2: EscreverCentralizada("C",x_enemy+25,y_enemy);break;
                        case 3: EscreverCentralizada("D",x_enemy+25,y_enemy);break;
                    }*/
                }
            }

            if(paused){
                gamepaused();
            }

            if(final_msg){
                Espera(3000);
                break;
            }

            if(game_over){
                gameover();
                final_msg = true;
            }
            if(win){
                game_win();
                final_msg = true;
            }
        }
        EncerraDesenho();

    }

    FinalizaJogo();

    return 0;
}
