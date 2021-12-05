#include "PIG.h"

PIG_Evento evento;          //evento ser tratado a cada passada do loop principal
PIG_Teclado meuTeclado;     //variável como mapeamento do teclado

// Tamanho do Mapa = 1024x768

int main( int argc, char* args[] ){

    //criando o jogo (aplicação)
    CriaJogo("Meu Jogo");

    int x_main_char, y_main_char = 0;
    //Define de quanto em quanto tempo é checado um input de movimento(diminuir o valor dessa variavel causa uma velocidade maior para o personagem)
    int move_check_time = 0.005;
    //associando o teclado (basta uma única vez) com a variável meuTeclado
    meuTeclado = GetTeclado();

    int timer = CriaTimer();

    //Gerando o sprite do personagem principal e aplicando suas dimensões
    int main_char = CriaSprite("..//images//eevee_sprite.jpg");
    SetDimensoesSprite(main_char,44,40);
    MoveSprite(main_char,0,0);

    //loop principal do jogo
    while(JogoRodando()){

        //pega um evento que tenha ocorrido desde a última passada do loop
        evento = GetEvento();

        GetXYSprite(main_char,&x_main_char,&y_main_char);

        //Checa por um input de teclado a cada X segundos e não movimenta o personagem caso ele se encontre na borda da janela
        if(TempoDecorrido(timer)>move_check_time){
            if(meuTeclado[PIG_TECLA_CIMA] && y_main_char <= 724){
                DeslocaSprite(main_char,0,+1);
            }
            if(meuTeclado[PIG_TECLA_BAIXO] && y_main_char >= 0){
                DeslocaSprite(main_char,0,-1);
            }
            if(meuTeclado[PIG_TECLA_ESQUERDA] && x_main_char >= 0){
                DeslocaSprite(main_char,-1,0);
            }
            if(meuTeclado[PIG_TECLA_DIREITA] && x_main_char <= 984){
                DeslocaSprite(main_char,+1,0);
            }
            ReiniciaTimer(timer);
        }

        //aqui o evento deve ser tratado e tudo deve ser atualizado

        //será feita a preparação do frame que será exibido na tela
        IniciaDesenho();

        //Desenha o mapa e o personagem principal
        DesenhaSpriteSimples("..//images//map_holder.png",0,0,0);
        DesenhaSprite(main_char);
        //todas as chamadas de desenho devem ser feitas aqui na ordem desejada

        //o frame totalmente pronto será mostrado na tela
        EncerraDesenho();
    }

    //o jogo será encerrado
    FinalizaJogo();

    return 0;
}
