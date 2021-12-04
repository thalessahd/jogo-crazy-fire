#include "PIG.h"

PIG_Evento evento;          //evento ser tratado a cada passada do loop principal
PIG_Teclado meuTeclado;     //vari�vel como mapeamento do teclado

int main( int argc, char* args[] ){

    //criando o jogo (aplica��o)
    CriaJogo("Meu Jogo");

    //associando o teclado (basta uma �nica vez) com a vari�vel meuTeclado
    meuTeclado = GetTeclado();

    //loop principal do jogo
    while(JogoRodando()){

        //pega um evento que tenha ocorrido desde a �ltima passada do loop
        evento = GetEvento();

        //aqui o evento deve ser tratado e tudo deve ser atualizado

        //ser� feita a prepara��o do frame que ser� exibido na tela
        IniciaDesenho();

        DesenhaRetangulo(200,200,50,90,VERMELHO);
        //todas as chamadas de desenho devem ser feitas aqui na ordem desejada

        //o frame totalmente pronto ser� mostrado na tela
        EncerraDesenho();
    }

    //o jogo ser� encerrado
    FinalizaJogo();

    return 0;
}
