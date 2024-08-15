#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

struct dadosmed
{
    char medico[25];
    char celular[12];
    char data_consulta[11];
};

typedef struct dadosmed dadosmed;

void inserirdados(dadosmed *pontdados)
{
    FILE * data;

    printf("Digite o nome do médico: ");
    fflush(stdin);
    gets(pontdados->medico);

    printf("Digite o numero do telefone do médico: ");
    fflush(stdin);
    gets(pontdados->celular);

    printf("Digite a data de sua consulta: ");
    fflush(stdin);
    gets(pontdados->data_consulta);

    data = fopen("basededados.txt","a");
    fwrite(pontdados, sizeof(*pontdados), 1, data);
    fclose(data);
}

void mostrardados(dadosmed *pontdados)
{
    FILE * data;

    int answer;

    data = fopen("basededados.txt","r");

    for(;;)
    {
        answer = fread(pontdados,sizeof(*pontdados),1,data);
        if(answer == 0)
        {
            break;
        }
        else
        {
            if(pontdados->medico[0]!='*')
            {
                printf("Médico: %s\n", pontdados->medico);
                printf("Celular: %s\n",pontdados->celular);
                printf("Data da consulta: %s\n\n",pontdados->data_consulta);
            }
        }
    }
    fclose(data);
}

int procurarmedico(dadosmed *pontdados, int type)
{
    FILE * data;

    int carac=0, answer, pos=0;

    char compareMed[25];

    printf("Digite o nome do médico que você deseja: ");
    fflush(stdin);
    gets(compareMed);

    data = fopen("basededados.txt","r");

    for(;;)
    {
        answer=fread(pontdados,sizeof(*pontdados),1,data);
        if(answer == 0)
        {
            break;
        }
        else
        {
            for(carac = 0; compareMed[carac] != '\0'; carac++)
            {
                if(compareMed[carac]!=pontdados->medico[carac])
                {
                    pos += 1;
                    break;
                }
            }
            if(compareMed[carac]=='\0'&&pontdados->medico[carac]=='\0'&&pontdados->medico[0]!='*')
            {
                if (type == 0) {
                    printf("\nMédico: %s - Celular: %s - Data da consulta: %s\n\n",compareMed,pontdados->celular,pontdados->data_consulta);
                    return pos;
                }
                else {
                    printf("\nMédico: %s - Celular: %s\n\n",compareMed,pontdados->celular);
                    return 0;
                }
            }
        }
    }

    printf("\nMédico não encontrado.\n\n");
    return -1;

    fclose(data);
}

void procurarcelular(dadosmed *pontdados)
{
    procurarmedico(pontdados, 1);
}

void procurardata(dadosmed *pontdados)
{
    FILE * data;

    int carac=0, feedback=0, answer;

    char compareDate[11];

    printf("Digite a data da consulta (dd/mm/yyyy): ");
    fflush(stdin);
    gets(compareDate);

    data = fopen("basededados.txt","r");

    for(;;)
    {
        answer = fread(pontdados,sizeof(*pontdados),1,data);
        if(answer == 0)
        {
            break;
        }
        else
        {
            for(carac = 0; compareDate[carac] != '\0'; carac++)
            {
                if(compareDate[carac]!=pontdados->data_consulta[carac])
                {
                    break;
                }
            }
            if(compareDate[carac]=='\0'&&pontdados->data_consulta[carac]=='\0'&&pontdados->medico[0]!='*')
            {
                printf("\nMédico: %s - Data da consulta: %s\n\n",pontdados->medico,pontdados->data_consulta);
                feedback=1;
            }
        }
    }
    if(feedback==0)
    {
        printf("\nConsulta não encontrada.\n\n");
    }
    fclose(data);
}

int alterardados(dadosmed *pontdados)
{
    FILE * data;

    char mname[25], mphone[11], mdata[11];
    char deletereg[2]={""};

    int carac=0, pos = 0, desl;

    pos = procurarmedico(pontdados, 0);

    if (pos == -1) return 0;

    printf("Digite o novo nome: ");
    fflush(stdin);
    gets(mname);
    printf("\nDigite o novo celular: ");
    fflush(stdin);
    gets(mphone);
    printf("\nDigite a nova data: ");
    fflush(stdin);
    gets(mdata);
    printf("\n");

    desl = pos * sizeof(*pontdados);

    for(int car=0;car<25;car++)
    {
        pontdados->medico[car]=deletereg[0];
        pontdados->medico[car]=mname[car];
    }

    for(int car=0;car<11;car++)
    {
        pontdados->celular[car]=deletereg[0];
        pontdados->celular[car]=mphone[car];
    }

    for(int car=0;car<11;car++)
    {
        pontdados->data_consulta[car]=deletereg[0];
        pontdados->data_consulta[car]=mdata[car];
    }

    data = fopen("basededados.txt","r+");
    fseek(data, desl, 0);
    fwrite(pontdados,sizeof(*pontdados),1,data);
    fclose(data);
}


void deletardados(dadosmed *pontdados)
{
    FILE * data;

    int sure, carac=0, pos=0, desl;

    pos = procurarmedico(pontdados, 0);

    if (pos == -1) return 0;
    do
    {
        printf("Você tem certeza que deseja excluir? 1-Sim, 2-Não: ");
        scanf(" %d",&sure);
        if(sure>2||sure<1)
        {
            printf("\n\nValor inválido.\n\n");
        }
    }
    while(sure<1||sure>2);
    printf("\n");
    if(sure==1)
    {
        desl = pos * sizeof(*pontdados);
        pontdados->medico[0]='*';
        data = fopen("basededados.txt","r+");
        fseek(data, desl, 0);
        fwrite(pontdados,sizeof(*pontdados),1,data);
        fclose(data);
        printf("Registro excluído.\n\n");
    }
}

int main()
{
    setlocale(LC_ALL,"Portuguese");

    struct dadosmed dados;
    int option;

    for(;;)

    {


        printf("------------------------------------------------------------------------------\n");
        printf("1 – Entrada de dados.\n");
        printf("2 – Lista todos os dados na tela.\n");
        printf("3 – Pesquisa um médico pelo nome completo e mostra todos os dados na tela.\n");
        printf("4 – Pesquisa um celular pelo nome completo.\n");
        printf("5 – Pesquisa pela data de consulta e mostra todas na tela.\n");
        printf("6 – Altera dados. Pesquisa pelo nome completo do médico.\n");
        printf("7 - Exclui dados. Pesquisa pelo nome completo do médico.\n");
        printf("8 - Saída.\n");
        printf("------------------------------------------------------------------------------\n");





        do
        {
            printf("\nEscolha uma opção: ");
            scanf(" %d",&option);
            if(option<1||option>8)
            {
                printf("\n\nValor inválido.\n\n");
            }
        }
        while(option<1||option>8);
        system("cls");

        switch(option)
        {

        case 1:
            inserirdados(&dados);
            system("cls");
            break;


        case 2:
            mostrardados(&dados);
            system("pause");
            system("cls");
            break;


        case 3:
            procurarmedico(&dados, 0);
            system("pause");
            system("cls");
            break;


        case 4:
            procurarcelular(&dados);
            system("pause");
            system("cls");
            break;


        case 5:
            procurardata(&dados);
            system("pause");
            system("cls");
            break;


        case 6:
            alterardados(&dados);
            system("pause");
            system("cls");
            break;


        case 7:
            deletardados(&dados);
            system("pause");
            system("cls");
            break;

        case 8:
            exit(0);
            break;
        }
    }
}
