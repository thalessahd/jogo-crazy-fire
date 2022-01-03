#include "PIG.h"

PIG_Evento evento;          //evento ser tratado a cada passada do loop principal
PIG_Teclado meuTeclado;     //variável como mapeamento do teclado

int map_height = 720;
// Tamanho do mapa = 800x600

//Desenha os retangulos para testes
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

//Desenha todos os elementos da hud
void hud_draw(int char_id, int hp, int extinguisher_id, int *ext, int *charge){
    int i =0;
    int posx[5] = {490,590,690,790,890};

    DesenhaObjeto(char_id);
    DesenhaRetangulo(60,PIG_ALT_TELA-40,30,hp,VERMELHO);

    //Desenha extintores na HUD
    for(i=0;i<5;i++){
        DesenhaObjeto(ext[i]);
        SetColoracaoObjeto(ext[i],BRANCO);
    }

    //Escrita da "munição" na tela
    for(i=0;i<5;i++){
        if(charge[i] < 25){
            EscreveInteiroEsquerda(charge[i],posx[i],PIG_ALT_TELA-50,VERMELHO);
        }else if(charge[i] >= 25 && charge[i] <= 50){
            EscreveInteiroEsquerda(charge[i],posx[i],PIG_ALT_TELA-50,AMARELO);
        }else{
            EscreveInteiroEsquerda(charge[i],posx[i],PIG_ALT_TELA-50,BRANCO);
        }
    }

    //Destaca o extintor selecionado
    SetColoracaoObjeto(ext[extinguisher_id-1],VERDE);
}

//Mensagem de game over
void gameover(){
    EscreverCentralizada("GAME OVER",PIG_LARG_TELA/2,map_height/2,VERMELHO);
}

//Inicia os timers controladores após o fim do tutorial
void starttimers(int t1, int t2, int t3, int t4, int t5, int t6){
    ReiniciaTimer(t1);
    ReiniciaTimer(t2);
    ReiniciaTimer(t3);
    ReiniciaTimer(t4);
    ReiniciaTimer(t5);
    ReiniciaTimer(t6);
}

//Escreve o tutorial na tela
void draw_tutorial(){
    EscreverCentralizada("TUTORIAL",PIG_LARG_TELA/2,PIG_ALT_TELA-100,VERMELHO);
    EscreverEsquerda("(1) - Extintor de Espuma -> Classes A e B",50,600,BRANCO);
    EscreverEsquerda("(2) - Extintor de Dioxido de Carbono -> Classes B e C",50,500,BRANCO);
    EscreverEsquerda("(3) - Extintor Quimico -> Classes B, C e D",50,400,BRANCO);
    EscreverEsquerda("(4) - Extintor de Agua -> Classe A",50,300,BRANCO);
    EscreverEsquerda("(5) - Extintor Especial -> Todas as Classes",50,200,BRANCO);
    EscreverCentralizada("APERTE ENTER PARA INICIAR",PIG_LARG_TELA/2,100,VERDE);
}

void game_win(){
    EscreverCentralizada("VITORIA",PIG_LARG_TELA/2,map_height/2,VERDE);
}

int main( int argc, char* args[] ){

    CriaJogo("Crazy Fire");

    //Variaveis do personagem principal
    int x_main_char = 0, y_main_char = 0, i = 0, x_enemy = 0, y_enemy = 0, kills = 0;
    int main_char_length = 40, main_char_height = 40, hp = 360, ext_id = 1, spawn_id = 0;

    //Munição inicial para cada extintor
    int charge[5] = {100,100,100,100,100}, enemy[10] = {0,0,0,0,0,0,0,0,0,0}, enemy_id[10] = {0,0,0,0,0,0,0,0,0,0}, extinguisher[5] = {0,0,0,0,0}, invis_wall[8] = {0,0,0,0,0,0,0,0};

    //Define de quanto em quanto tempo é checado um input de movimento, a diminuição da munição, o spawn de recarga, o spawn de inimigos e o dano sofrido, respectivamente.
    float move_check_time = 0.002, empty_ext_time = 0.01, reload_spawn_time = 5, enemy_spawn_time = 3, enemy_move_time = 0.008, damage_taken_time = 0.025;

    bool draw_smoke = false, draw_reload = false, game_over = false, final_msg = false, tutorial = true, win = false;
    bool draw_enemy[10] = {false,false,false,false,false,false,false,false,false,false};

    meuTeclado = GetTeclado();

    int timer = CriaTimer(1);            //Para checar movimento
    int timershot = CriaTimer(1);        //Para checar disparo do extintor
    int timer_reload = CriaTimer(1);     //Para checar spawn do extintor de recarga
    int timer_enemy = CriaTimer(1);      //Para checar spawn dos inimigos
    int timer_enemy_move = CriaTimer(1); //Para checar movimento do inimigo
    int timer_damage = CriaTimer(1);     //Para checar dano sofrido

    //Criando imagens dos extintores para a HUD
    for(i=0;i<5;i++){
        extinguisher[i] = CriaObjeto("..//images//extinguisher.png");
        SetDimensoesObjeto(extinguisher[i],50,50);
        MoveObjeto(extinguisher[i],450+100*i,PIG_ALT_TELA-50);
    }

    int reload = CriaObjeto("..//images//extinguisher.png");
    SetDimensoesObjeto(reload,40,40);

    //Criando imagem do personagem para a HUD
    int hudchar = CriaObjeto("..//images//char_front.jpg",1);
    SetDimensoesObjeto(hudchar,50,50);
    MoveObjeto(hudchar,0,PIG_ALT_TELA-50);

    //Criando fumaça do extintor
    int smoke = CriaObjeto("..//images//smoke.png");
    SetDimensoesObjeto(smoke,50,50);

    for(i=0;i<10;i++){
        enemy[i] = CriaObjeto("..//images//flame.png");
        SetDimensoesObjeto(enemy[i],50,50);
    }

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
        if(meuTeclado[PIG_TECLA_5]){
            ext_id = 5;
        }

        //Checa disparo do extintor e desenha a fumaça de acordo com a direção do personagem
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

        //Spawna extintor de recarga
        if(TempoDecorrido(timer_reload) > reload_spawn_time){
            MoveObjeto(reload,rand()%800+50,rand()%600+50);
            draw_reload = true;
            ReiniciaTimer(timer_reload);
        }

        //Fecha o jogo apertando ESC
        if(meuTeclado[PIG_TECLA_ESC]){
            break;
        }

        //Pegar recarga de extintor no mapa
        if(TestaColisaoObjetos(main_char,reload) && draw_reload){
            for(i=0;i<5;i++){
                if(charge[i]>=75){
                    charge[i] = 100;
                }else{
                    charge[i] = charge[i] + 25;
                }
            }
            draw_reload = false;
            ReiniciaTimer(timer_reload);
        }

        //Spawn de inimigos
        if(TempoDecorrido(timer_enemy) > enemy_spawn_time){
            spawn_id = rand()%4;

            for(i=0;i<10;i++){
                if(!draw_enemy[i]){
                    enemy_id[i] = rand()%4;
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
                    if(ext_id == 5){
                        draw_enemy[i] = false;
                        MoveObjeto(enemy[i],0,PIG_ALT_TELA);
                        //DestroiObjeto(enemy[i]);
                        kills++;
                    }
                }
            }
        }

        //Condição de vitória
        if(kills >= 10){
            win = true;
        }

        //Condição de game over
        if(hp <= 0){
            game_over = true;
        }

        IniciaDesenho();

        //Começa mostrando o tutorial na tela
        if(tutorial){
            draw_tutorial();
            if(meuTeclado[PIG_TECLA_ENTER]){
                tutorial = false;
                starttimers(timer,timershot,timer_damage,timer_enemy,timer_enemy_move,timer_reload);
            }
        }else{

            //Desenha o mapa
            DesenhaSpriteSimples("..//images//map_holder.png",0,0,0);

            //Desenha HUD e retangulos para teste
            test_draw();
            hud_draw(hudchar,hp,ext_id,extinguisher,charge);

            //Desenha personagem principal
            DesenhaObjeto(main_char);

            //Desenha fumaça do extintor e extintor de recarga
            if(draw_smoke){
                DesenhaObjeto(smoke);
                draw_smoke = false;
            }
            if(draw_reload){
                DesenhaObjeto(reload);
            }

            //Desenha inimigos
            for(i=0;i<10;i++){
                if(draw_enemy[i]){
                    DesenhaObjeto(enemy[i]);
                    GetXYObjeto(enemy[i],&x_enemy,&y_enemy);
                    switch(enemy_id[i]){
                        case 0: EscreverCentralizada("A",x_enemy+25,y_enemy);break;
                        case 1: EscreverCentralizada("B",x_enemy+25,y_enemy);break;
                        case 2: EscreverCentralizada("C",x_enemy+25,y_enemy);break;
                        case 3: EscreverCentralizada("D",x_enemy+25,y_enemy);break;
                    }
                }
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
