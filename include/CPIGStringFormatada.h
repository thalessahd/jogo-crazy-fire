#ifndef _CPIGSTRINGFORMATADA_
#define _CPIGSTRINGFORMATADA_

class CPIGStringFormatada{

    char *ExtraiString(){
        char *resp = (char*)malloc(sizeof(letras.size()+1));
        unsigned int i=0;
        while (i<letras.size()){
            resp[i] = letras[i];
            i++;
        }
        resp[i]='\0';
        return resp;
    }

    string letras;
    vector<int> largAcumulada;
    vector<PIGCor> cores;
    vector<PIGEstilo> estilos;

public:

    ~CPIGStringFormatada(){
        Clear();
    }

    Uint16 GetLetra(int indice){
        return letras[indice];
    }

    PIGCor GetCor(int indice){
        return cores[indice];
    }

    PIGEstilo GetEstilo(int indice){
        return estilos[indice];
    }

    void Clear(){
        letras = "";
        largAcumulada.clear();
        cores.clear();
        estilos.clear();
    }

    void Adiciona(char letra, int larguraAcumulada, PIGCor cor, PIGEstilo estilo){
        letras += letra;
        largAcumulada.push_back(larguraAcumulada);
        cores.push_back(cor);
        estilos.push_back(estilo);
    }


    /*void Print(){
        EXECUTA_SE_DEBUG(printf("__%s__ (%d)\n",letras.c_str(),LargTotalPixels()));
    }*/

    int LargTotalPixels(){
        if (letras.size()>0)
            return largAcumulada[letras.size()-1];
        return 0;
    }

    int size(){
        return letras.size();
    }

    vector<CPIGStringFormatada> SeparaPalavras(string delim){
        vector<CPIGStringFormatada> resp;
        if (letras.size()==0) return resp;
        unsigned int indice;
        int largBase=0;
        CPIGStringFormatada strAtual;
        for (unsigned int i=0;i<letras.size();i++){

            indice = delim.find(letras[i]);
            if (indice != std::string::npos){//achou delimitadores
                resp.push_back(strAtual);
                //strAtual.Print();
                strAtual.Clear();
                strAtual.Adiciona(letras[i],largAcumulada[i]-largBase,cores[i],estilos[i]);
                if (letras[i]!='\n'){
                    resp.push_back(strAtual);
                }

                strAtual.Clear();
                largBase = largAcumulada[i];
            }else strAtual.Adiciona(letras[i],largAcumulada[i]-largBase,cores[i],estilos[i]);
        }
        if (strAtual.letras!=""){
            resp.push_back(strAtual);
            //strAtual.Print();
        }
        return resp;
    }

    vector<CPIGStringFormatada> ExtraiLinhas(int largMax, string delim){
        vector<CPIGStringFormatada> resp;
        if (letras.size()==0) return resp;
        vector<CPIGStringFormatada> palavras = SeparaPalavras(delim);

        CPIGStringFormatada linhaAtual = palavras[0];   //linha atual (que est� sendo montada) cont�m pelo menos a primeira palavra

        for (unsigned int i=1;i<palavras.size();i++){
            CPIGStringFormatada palavra = palavras[i];   //pega a pr�xima palavra

            if (linhaAtual.LargTotalPixels() + palavra.LargTotalPixels() > largMax){//a palavra estouraria a largura m�xima se fosse agregada                if (ttttt==0){
                resp.push_back(linhaAtual); //coloca a linha que est� montada no vetor de linhas
                linhaAtual = palavra; //a palavra que estouraria o limite come�a a pr�xima linha
            }else{//n�o estourou o limite
                linhaAtual += palavra;
            }

            if (palavra.letras[palavra.letras.size()-1]=='\n'){//se existe uma quebra de linha for�ada
                resp.push_back(linhaAtual);

                if (++i<palavras.size()){
                    linhaAtual = palavras[i]; //come�a uma nova linha com a p�roxima palavra
                }else linhaAtual.letras = "";
            }
        }

        if (linhaAtual.letras != ""){
            resp.push_back(linhaAtual); //pega a linha que sobrou do processamento (�ltima linha que n�o foi quebrada)
        }

        palavras.clear();
        return resp;
    }

    CPIGStringFormatada operator +=(CPIGStringFormatada outra){
        int largAtual = LargTotalPixels();
        for (unsigned int i=0;i<outra.letras.size();i++){
            Adiciona(outra.letras[i],outra.largAcumulada[i]+largAtual,outra.cores[i],outra.estilos[i]);
        }
        return *this;
    }
};

#endif //_CPIGSTRINGFORMATADA_
