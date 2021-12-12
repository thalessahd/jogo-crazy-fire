#include "PIG.h"

PIG_Evento evento;          //evento ser tratado a cada passada do loop principal
PIG_Teclado meuTeclado;     //variável como mapeamento do teclado

int map_height = 720;
// Tamanho do mapa = 960x720

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
void hud_draw(int char_id, int hp, int extinguisher_id, int ext1, int ext2, int ext3, int ext4, int ext5, int *charge){
    int i =0;
    int posx[5] = {490,590,690,790,890};

    DesenhaObjeto(char_id);
    DesenhaRetangulo(60,PIG_ALT_TELA-40,30,hp,VERMELHO);
    DesenhaObjeto(ext1);
    DesenhaObjeto(ext2);
    DesenhaObjeto(ext3);
    DesenhaObjeto(ext4);
    DesenhaObjeto(ext5);
    SetColoracaoObjeto(ext1,BRANCO);
    SetColoracaoObjeto(ext2,BRANCO);
    SetColoracaoObjeto(ext3,BRANCO);
    SetColoracaoObjeto(ext4,BRANCO);
    SetColoracaoObjeto(ext5,BRANCO);

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
    switch(extinguisher_id){
        case 1: SetColoracaoObjeto(ext1,VERDE);break;
        case 2: SetColoracaoObjeto(ext2,VERDE);break;
        case 3: SetColoracaoObjeto(ext3,VERDE);break;
        case 4: SetColoracaoObjeto(ext4,VERDE);break;
        case 5: SetColoracaoObjeto(ext5,VERDE);break;
    }
}

int main( int argc, char* args[] ){

    CriaJogo("Crazy Fire");

    //Variaveis do personagem principal
    int x_main_char, y_main_char = 0, i = 0;
    int main_char_length = 40, main_char_height = 40, hp = 360, ext_id = 1;

    //Munição inicial para cada extintor
    int charge[5] = {100,100,100,100,100};

    //Define de quanto em quanto tempo é checado um input de movimento, a diminuição da munição e o spawn de recarga
    float move_check_time = 0.002, empty_ext_time = 0.07, reload_spawn_time = 5;

    bool draw_smoke = false, draw_reload = false;

    meuTeclado = GetTeclado();

    int timer = CriaTimer();         //Para checar movimento
    int timershot = CriaTimer();     //Para checar disparo do extintor
    int timer_reload = CriaTimer();  //Para checar spawn do extintor de recarga

    //Criando imagens dos extintores para a HUD
    int extinguisher1 = CriaObjeto("..//images//extinguisher.png");
    int extinguisher2 = CriaObjeto("..//images//extinguisher.png");
    int extinguisher3 = CriaObjeto("..//images//extinguisher.png");
    int extinguisher4 = CriaObjeto("..//images//extinguisher.png");
    int extinguisher5 = CriaObjeto("..//images//extinguisher.png");
    SetDimensoesObjeto(extinguisher1,50,50);
    SetDimensoesObjeto(extinguisher2,50,50);
    SetDimensoesObjeto(extinguisher3,50,50);
    SetDimensoesObjeto(extinguisher4,50,50);
    SetDimensoesObjeto(extinguisher5,50,50);
    MoveObjeto(extinguisher1,450,PIG_ALT_TELA - 50);
    MoveObjeto(extinguisher2,550,PIG_ALT_TELA - 50);
    MoveObjeto(extinguisher3,650,PIG_ALT_TELA - 50);
    MoveObjeto(extinguisher4,750,PIG_ALT_TELA - 50);
    MoveObjeto(extinguisher5,850,PIG_ALT_TELA - 50);

    int reload = CriaObjeto("..//images//extinguisher.png");
    SetDimensoesObjeto(reload,40,40);

    //Criando imagem do personagem para a HUD
    int hudchar = CriaObjeto("..//images//char_front.jpg",1);
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
    int invis_wall_bot = CriaObjeto("..//images//branco.png",0);
    int invis_wall_left = CriaObjeto("..//images//branco.png",0);
    int invis_wall_bot2 = CriaObjeto("..//images//branco.png",0);
    int invis_wall_top  = CriaObjeto("..//images//branco.png",0);
    int invis_wall_top2 = CriaObjeto("..//images//branco.png",0);      //Por algum motivo criar apenas um e replicar para o resto n estava funcionando
    int invis_wall_left2  = CriaObjeto("..//images//branco.png",0);    //Fazer um por um foi o unico jeito que rodou aqui
    int invis_wall_right  = CriaObjeto("..//images//branco.png",0);
    int invis_wall_right2 = CriaObjeto("..//images//branco.png",0);

    SetDimensoesObjeto(invis_wall_bot,40,(PIG_LARG_TELA/2)-60);
    SetDimensoesObjeto(invis_wall_bot2,40,(PIG_LARG_TELA/2)-60);
    SetDimensoesObjeto(invis_wall_top,40,(PIG_LARG_TELA/2)-60);
    SetDimensoesObjeto(invis_wall_top2,40,(PIG_LARG_TELA/2)-60);

    MoveObjeto(invis_wall_bot,0,0);
    MoveObjeto(invis_wall_bot2,(PIG_LARG_TELA/2)+60,0);
    MoveObjeto(invis_wall_top,0,map_height-40);
    MoveObjeto(invis_wall_top2,(PIG_LARG_TELA/2)+60,map_height-40);

    SetDimensoesObjeto(invis_wall_left,(map_height/2)-60,40);
    SetDimensoesObjeto(invis_wall_left2,(map_height/2)-60,40);
    SetDimensoesObjeto(invis_wall_right,(map_height/2)-60,40);
    SetDimensoesObjeto(invis_wall_right2,(map_height/2)-60,40);

    MoveObjeto(invis_wall_left2,0,(map_height/2)+60);
    MoveObjeto(invis_wall_right,PIG_LARG_TELA-40,0);
    MoveObjeto(invis_wall_right2,PIG_LARG_TELA-40,(map_height/2)+60);

    while(JogoRodando()){

        evento = GetEvento();

        GetXYObjeto(main_char,&x_main_char,&y_main_char); //Variaveis que guardam a posição atual do personagem principal

        //Checa por um input de teclado a cada X segundos e não movimenta o personagem caso ele se encontre em colisão com o mapa | muda o sprite do personagem de acordo com a direção do movimento
        if(TempoDecorrido(timer) > move_check_time){
            if(y_main_char < map_height-40 && (meuTeclado[PIG_TECLA_CIMA] || meuTeclado[PIG_TECLA_w]) && (!TestaColisaoObjetos(main_char,invis_wall_top) && !TestaColisaoObjetos(main_char,invis_wall_top2) && (!TestaColisaoObjetos(main_char,invis_wall_left2) || x_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall_right2) || x_main_char <=PIG_LARG_TELA-79))){
                DeslocaObjeto(main_char,0,+1);
                MudaFrameObjeto(main_char,2);
            }
            if(y_main_char > 0 && (meuTeclado[PIG_TECLA_BAIXO] || meuTeclado[PIG_TECLA_s]) && (!TestaColisaoObjetos(main_char,invis_wall_bot) && !TestaColisaoObjetos(main_char,invis_wall_bot2) && (!TestaColisaoObjetos(main_char,invis_wall_left) || x_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall_right) || x_main_char <=PIG_LARG_TELA-79))){
                DeslocaObjeto(main_char,0,-1);
                MudaFrameObjeto(main_char,1);
            }
            if(x_main_char > 0 && (meuTeclado[PIG_TECLA_ESQUERDA] || meuTeclado[PIG_TECLA_a]) && (!TestaColisaoObjetos(main_char,invis_wall_left) && !TestaColisaoObjetos(main_char,invis_wall_left2) && (!TestaColisaoObjetos(main_char,invis_wall_bot) || y_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall_top) || y_main_char <= map_height-79))){
                DeslocaObjeto(main_char,-1,0);
                MudaFrameObjeto(main_char,4);
            }
            if(x_main_char < PIG_LARG_TELA-40 && (meuTeclado[PIG_TECLA_DIREITA] || meuTeclado[PIG_TECLA_d]) && (!TestaColisaoObjetos(main_char,invis_wall_right) && !TestaColisaoObjetos(main_char,invis_wall_right2) && (!TestaColisaoObjetos(main_char,invis_wall_bot2) || y_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall_top2) || y_main_char <= map_height-79))){
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

        IniciaDesenho();

        //Desenha o mapa
        DesenhaSpriteSimples("..//images//map_holder.png",0,0,0);

        //Desenha HUD e retangulos para teste
        test_draw();
        hud_draw(hudchar,hp,ext_id,extinguisher1,extinguisher2,extinguisher3,extinguisher4,extinguisher5,charge);

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

        EncerraDesenho();

    }

    FinalizaJogo();

    return 0;
}
