#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <conio.h>
#include <math.h>

// Variaveis Globais
HANDLE h;
HANDLE trava;
int sorteio;
int posicaotiro=13;
int podeatirar=0;
int matatiro=0;
int vida=1;
int linhatiro;
int posicaoheli;
int soltainimigo=1;
//Marcador do vetor do tiro

//Antiga posicao do helicoptero
int oldlinhafogo=4;

// Marcador de Pontuação do Jogo
int pontos=0;

//Indentifica a linha do helicoptero
int linhafogo=4;

// Movimenta o Cursor na tela
void gotoxy(int x,int y)
{
    COORD coord;
    coord.X=(short)x;
    coord.Y=(short)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

// Funçao que deixa a aparencia do prinft colorida
void ligacor(int fgcolor,int bgcolor)
{
    bgcolor*=pow(2,4);
    SetConsoleTextAttribute ( h, fgcolor | bgcolor);
}

// Funçao que remove a aparencia do prinft e deixa ele como default
void desligacor()
{
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;
    SetConsoleTextAttribute ( h, wOldColorAttrs);
}

//Função que monta as bordas do jogo
void borda()
{
    int coluna=0, linha=0;
    while(linha<20)
    {
        gotoxy(coluna,linha);
        printf("%c",178);
        gotoxy(coluna+100,linha);
        printf("%c",178);
        linha++;
    }
    coluna=1;
    linha=0;
    while(coluna<100)
    {
        gotoxy(coluna,linha);
        printf("%c",178);
        gotoxy(coluna,linha+19);
        printf("%c",178);
        coluna++;
    }
}


void apaga(int posicao[])
{
    WaitForSingleObject(trava,INFINITE);
    gotoxy(posicao[1],posicao[2]);
    printf("          ");
    gotoxy(posicao[1],posicao[2]+1);
    printf("          ");
    ReleaseMutex(trava);
}

void desenhaheli(int posicao[])
{
    WaitForSingleObject(trava,INFINITE);
    if(posicao[0]==1)
    {
        gotoxy(posicao[1],posicao[2]);
        printf("   ___.___");
        gotoxy(posicao[1],posicao[2]+1);
        printf("---='[_]b ");
        posicao[0]=2;
    }
    else if(posicao[0]==2)
    {
        gotoxy(posicao[1],posicao[2]);
        printf("      |   ");
        gotoxy(posicao[1],posicao[2]+1);
        printf("/--='[|]b ");
        posicao[0]=3;
    }
    else if(posicao[0]==3)
    {
        gotoxy(posicao[1],posicao[2]);
        printf("   ___.___");
        gotoxy(posicao[1],posicao[2]+1);
        printf("|--='[_]b");
        posicao[0]=4;
    }
    else if(posicao[0]==4)
    {
        gotoxy(posicao[1],posicao[2]);
        printf("   ___.___");
        gotoxy(posicao[1],posicao[2]+1);
        printf("---='[_]b ");
        posicao[0]=1;
    }
    ReleaseMutex(trava);
}

//Verifica onde o helicoptero ficara
void verificapos(int posicao[])
{
    if(oldlinhafogo!=linhafogo)
    {
        apaga(posicao);
    }
    if(linhafogo==1)
    {
        posicao[2]=2;
        posicaoheli=1;
    }
    else if(linhafogo==2)
    {
        posicao[2]=4;
        posicaoheli=2;
    }
    else if(linhafogo==3)
    {
        posicao[2]=6;
        posicaoheli=3;
    }
    else if(linhafogo==4)
    {
        posicao[2]=8;
        posicaoheli=4;
    }
    else if(linhafogo==5)
    {
        posicao[2]=10;
        posicaoheli=5;
    }
    else if(linhafogo==6)
    {
        posicao[2]=12;
        posicaoheli=6;
    }
    else if(linhafogo==7)
    {
        posicao[2]=14;
        posicaoheli=7;
    }
    else if(linhafogo==8)
    {
        posicao[2]=16;
        posicaoheli=8;
    }
    oldlinhafogo=linhafogo;
}

void helicoptero()
{
    int coluna=1,linha=1;
    int posicao[3];
    posicao[1]=3;
    posicao[0]=1; //Posicao da Helice
    while(vida==1)
    {
        verificapos(posicao);
        desenhaheli(posicao);
        _sleep(50); //Tempo de rotação do helicoptero
    }
    while(linha<18)
    {
    gotoxy(coluna,linha);
    printf("                                         ");
    linha++;
    }
    gotoxy(20,10);
    ligacor(12,0);
    printf("    GAME OVER!");
    gotoxy(20,11);
    ligacor(15,0);
    printf("-------------------",pontos);
    gotoxy(20,12);
    ligacor(11,0);
    printf("VOCE FEZ %d PONTOS",pontos);
}

void pontuacao()
{
    int oldpontos=0; // Variavel de Comparação
    WaitForSingleObject(trava,INFINITE);
    gotoxy(6,21);
    printf("Pontua%c%co",135,198);
    ReleaseMutex(trava);
    while(vida==1)
    {
        if(pontos!=oldpontos)
        {
            WaitForSingleObject(trava,INFINITE);
            gotoxy(9,22);
            ligacor(0,15);
            printf(" %d ",pontos);
            ligacor(15,0);
            ReleaseMutex(trava);
        }
    }
}

void inimigo()
{
    int linha;
    int coluna=99;
    int matabixo=0;
    int bixolinha;
    int cordobixo=0;
    int velocidade=70;
    if(sorteio==1)
    {
        linha=3;
        bixolinha=1;
    }
    else if(sorteio==2)
    {
        linha=5;
        bixolinha=2;
    }
    else if(sorteio==3)
    {
        linha=7;
        bixolinha=3;
    }
    else if(sorteio==4)
    {
        linha=9;
        bixolinha=4;
    }
    else if(sorteio==5)
    {
        linha=11;
        bixolinha=5;
    }
    else if(sorteio==6)
    {
        linha=13;
        bixolinha=6;
    }
    else if(sorteio==7)
    {
        linha=15;
        bixolinha=7;
    }
    else if(sorteio==8)
    {
        linha=17;
        bixolinha=8;
    }
    int i;
    while(coluna>1 && matabixo<2 && vida==1)
    {
        if(coluna==80)
        {
            soltainimigo++;
        }
        WaitForSingleObject(trava,INFINITE);
        gotoxy(coluna,linha);
        if(cordobixo==1)
        {
            ligacor(11,0);
            printf("%c",279);
            ligacor(15,0);
        }
        if(cordobixo==0)
        {
            ligacor(11,0);
            printf("%c",178);
            ligacor(15,0);
        }
        ReleaseMutex(trava);
        _sleep(velocidade);
        WaitForSingleObject(trava,INFINITE);
        gotoxy(coluna,linha);
        printf(" ");
        ReleaseMutex(trava);
        coluna--;
        WaitForSingleObject(trava,INFINITE);
        if((coluna==posicaotiro && bixolinha==linhatiro)||(coluna==posicaotiro+1 && bixolinha==linhatiro)||(coluna==posicaotiro-1 && bixolinha==linhatiro))
        {
            matatiro=1;
            matabixo=2;
            cordobixo=1;
            posicaotiro=299;
            velocidade=10;
            pontos++;
        }
        else if(bixolinha==posicaoheli && coluna==13 )
        {
            vida=0;
            matabixo=2;
        }
        ReleaseMutex(trava);
    }
    if(coluna==1){
        vida=0;
    }
    coluna=300;

}

void inimigos()
{
    int linha;
    while(vida==1)
    {
        if(soltainimigo>0)
        {
            sorteio=1+rand()%7;
            soltainimigo--;
            _beginthread(inimigo,NULL,NULL);
        }
    }
}

void atirar()
{
    matatiro=0;
    int posicao[4];
    int coluna=13;
    int linha;
    posicaotiro=13;
    matatiro=0;
    if(linhafogo==1)
    {
        linha=3;
        linhatiro=1;
    }
    else if(linhafogo==2)
    {
        linha=5;
        linhatiro=2;
    }
    else if(linhafogo==3)
    {
        linha=7;
        linhatiro=3;
    }
    else if(linhafogo==4)
    {
        linha=9;
        linhatiro=4;
    }
    else if(linhafogo==5)
    {
        linha=11;
        linhatiro=5;
    }
    else if(linhafogo==6)
    {
        linha=13;
        linhatiro=6;
    }
    else if(linhafogo==7)
    {
        linha=15;
        linhatiro=7;
    }
    else if(linhafogo==8)
    {
        linha=17;
        linhatiro=8;
    }
    while(posicaotiro<40 && matatiro==0)
    {
        WaitForSingleObject(trava,INFINITE);
        gotoxy(coluna,linha);
        ligacor(14,0);
        printf("-");
        ligacor(15,0);
        ReleaseMutex(trava);
        _sleep(50);
        WaitForSingleObject(trava,INFINITE);
        gotoxy(coluna,linha);
        printf(" ");
        coluna++;
        posicaotiro++;
        podeatirar=1;
        ReleaseMutex(trava);
    }
    posicaotiro=299;
    podeatirar=0;


}

//Função que reconhece a tecla digitada pelo jogador
void pegatecla()
{
    char tecla;
    while(vida==1)
    {
        tecla=getch();
        WaitForSingleObject(trava,INFINITE);
        if(tecla=='w' && linhafogo>1)
        {
            linhafogo--;
        }
        if(tecla=='s' && linhafogo<9)
        {
            linhafogo++;
        }
        if(tecla==32)
        {
            if(podeatirar<1)
            {
                _beginthread(atirar,NULL,NULL);
            }
        }
        ReleaseMutex(trava);
    }
}

void main()
{
    // Inicialização
    h = GetStdHandle ( STD_OUTPUT_HANDLE );
    trava=CreateMutex(NULL,FALSE,NULL);
    srand (time(0));
    // Funções que serão executadas
    borda(); //Contorno da tela
    _beginthread(pontuacao,NULL,NULL); //Pontos na tela
    _beginthread(helicoptero,NULL,NULL); //Funçao que cria o helicoptero
    _beginthread(pegatecla,NULL,NULL);
    _beginthread(inimigos,NULL,NULL);

    while(1);

}
