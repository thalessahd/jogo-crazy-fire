#include "PIG.h"

PIG_Evento evento;          //evento ser tratado a cada passada do loop principal
PIG_Teclado meuTeclado;     //variável como mapeamento do teclado

// Tamanho do Mapa = 1024x768

int main( int argc, char* args[] ){

    CriaJogo("Crazy Fire");

    int x_main_char, y_main_char = 0;
    //Define de quanto em quanto tempo é checado um input de movimento(diminuir o valor dessa variavel causa uma velocidade maior para o personagem)
    int move_check_time = 0.005;

    meuTeclado = GetTeclado();

    int timer = CriaTimer();

    //Gerando o sprite do personagem principal e aplicando suas dimensões
    int main_char = CriaObjeto("..//images//eevee_sprite.jpg");
    SetDimensoesObjeto(main_char,44,40);
    MoveObjeto(main_char,PIG_LARG_TELA/2,PIG_ALT_TELA/2);

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
    MoveObjeto(invis_wall_top,0,PIG_ALT_TELA-40);
    MoveObjeto(invis_wall_top2,(PIG_LARG_TELA/2)+60,PIG_ALT_TELA-40);

    SetDimensoesObjeto(invis_wall_left,(PIG_ALT_TELA/2)-60,40);
    SetDimensoesObjeto(invis_wall_left2,(PIG_ALT_TELA/2)-60,40);
    SetDimensoesObjeto(invis_wall_right,(PIG_ALT_TELA/2)-60,40);
    SetDimensoesObjeto(invis_wall_right2,(PIG_ALT_TELA/2)-60,40);

    MoveObjeto(invis_wall_left2,0,(PIG_ALT_TELA/2)+60);
    MoveObjeto(invis_wall_right,PIG_LARG_TELA-40,0);
    MoveObjeto(invis_wall_right2,PIG_LARG_TELA-40,(PIG_ALT_TELA/2)+60);

    while(JogoRodando()){

        evento = GetEvento();

        GetXYObjeto(main_char,&x_main_char,&y_main_char);

        //Checa por um input de teclado a cada X segundos e não movimenta o personagem caso ele se encontre na em colisão com o mapa
        if(TempoDecorrido(timer)>move_check_time){
            if(y_main_char < PIG_ALT_TELA-40 && (meuTeclado[PIG_TECLA_CIMA] || meuTeclado[PIG_TECLA_w]) && (!TestaColisaoObjetos(main_char,invis_wall_top) && !TestaColisaoObjetos(main_char,invis_wall_top2) && (!TestaColisaoObjetos(main_char,invis_wall_left2) || x_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall_right2) || x_main_char <=PIG_LARG_TELA-79))){
                DeslocaObjeto(main_char,0,+1);
            }
            if(y_main_char > 0 && (meuTeclado[PIG_TECLA_BAIXO] || meuTeclado[PIG_TECLA_s]) && (!TestaColisaoObjetos(main_char,invis_wall_bot) && !TestaColisaoObjetos(main_char,invis_wall_bot2) && (!TestaColisaoObjetos(main_char,invis_wall_left) || x_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall_right) || x_main_char <=PIG_LARG_TELA-79))){
                DeslocaObjeto(main_char,0,-1);
            }
            if(x_main_char > 0 && (meuTeclado[PIG_TECLA_ESQUERDA] || meuTeclado[PIG_TECLA_a]) && (!TestaColisaoObjetos(main_char,invis_wall_left) && !TestaColisaoObjetos(main_char,invis_wall_left2) && (!TestaColisaoObjetos(main_char,invis_wall_bot) || y_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall_top) || y_main_char <= PIG_ALT_TELA-80))){
                DeslocaObjeto(main_char,-1,0);
            }
            if(x_main_char < PIG_LARG_TELA-40 && (meuTeclado[PIG_TECLA_DIREITA] || meuTeclado[PIG_TECLA_d]) && (!TestaColisaoObjetos(main_char,invis_wall_right) && !TestaColisaoObjetos(main_char,invis_wall_right2) && (!TestaColisaoObjetos(main_char,invis_wall_bot2) || y_main_char >=39) && (!TestaColisaoObjetos(main_char,invis_wall_top2) || y_main_char <= PIG_ALT_TELA-80))){
                DeslocaObjeto(main_char,+1,0);
            }
            ReiniciaTimer(timer);
        }


        if(meuTeclado[PIG_TECLA_ESC]){
            break;
        }
        IniciaDesenho();

        //Desenha o mapa e o personagem principal
        DesenhaSpriteSimples("..//images//map_holder.png",0,0,0);
        //Retangulos para visualização dos testes de colisão
        DesenhaRetangulo(0,0,40,PIG_LARG_TELA,VERMELHO);
        DesenhaRetangulo(0,0,PIG_ALT_TELA,40,VERMELHO);
        DesenhaRetangulo(PIG_LARG_TELA-40,0,PIG_ALT_TELA,40,VERMELHO);
        DesenhaRetangulo(0,PIG_ALT_TELA-40,40,PIG_LARG_TELA,VERMELHO);
        DesenhaRetangulo((PIG_LARG_TELA/2)-60,0,40,120,VERDE);
        DesenhaRetangulo(0,(PIG_ALT_TELA/2)-60,120,40,VERDE);
        DesenhaRetangulo((PIG_LARG_TELA/2)-60,PIG_ALT_TELA-40,40,120,VERDE);
        DesenhaRetangulo(PIG_LARG_TELA-40,(PIG_ALT_TELA/2)-60,120,40,VERDE);
        DesenhaObjeto(main_char);


        EncerraDesenho();
    }

    FinalizaJogo();

    return 0;
}
