#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Funções para criptografia e descriptografia
void Criptografar(char chave[], char texto[], int texto_cript[]) {
    int tam_chave = strlen(chave);
    int valor_chave, aux = 0;

    for (int i = 0; i < strlen(texto); i++) {
        texto_cript[i] = texto[i];
    }

    aux = 0;
    while (aux < strlen(texto)) {
        for (int i = 0; i < tam_chave && aux < strlen(texto); i++) {
            valor_chave = chave[i];
            texto_cript[aux] *= valor_chave;
            aux++;
        }
    }
}

void SalvarArquivo(int texto_cript[], int tamanho, const char *nome_arquivo) {
    FILE* arquivo = fopen("usuario.txt", "w");
    if (arquivo == NULL) {
        printf("\n\nErro ao criar o arquivo....\n\n\n");
        return;
    }
    for (int i = 0; i < tamanho; i++) {
        fprintf(arquivo, "%d ", texto_cript[i]);
    }
    fclose(arquivo);
}

void CriptografarTexto(char chave[]) {
    char texto[100000];
    int texto_cript[100000];

    printf("\n\nDigite o texto a ser criptografado: \n");
    fgets(texto, sizeof(texto), stdin);
    texto[strcspn(texto, "\n")] = 0;

    Criptografar(chave, texto, texto_cript);
    SalvarArquivo(texto_cript, strlen(texto), "criptografado.txt");
}

void Decriptografar(char chave[]) {
    int tam_chave = strlen(chave);
    int texto_decript[10000];
    int valor, pos_chave = 0, pos_texto = 0;

    FILE* arquivo = fopen("criptografado.txt", "r");
    if (arquivo == NULL) {
        printf("\n\nErro ao abrir o arquivo......\n\n\n");
        return;
    }
    while (fscanf(arquivo, "%d", &valor) == 1) {
        valor /= chave[pos_chave];
        texto_decript[pos_texto] = (char)valor;
        pos_texto++;

        pos_chave = (pos_chave + 1) % tam_chave;
    }
    fclose(arquivo);

    FILE* saida = fopen("descriptografado.txt", "w");
    for (int i = 0; i < pos_texto; i++) {
        fputc(texto_decript[i], saida);
    }
    fclose(saida);
    printf("\n\nO arquivo foi descriptografado com sucesso! \n\n\n");
}

// Funções de gerenciamento de usuários
void SolicitarDadosUsuario(char chave[]) {
    char nome[100], senha[100];
    int texto_cript[100];

    printf("Digite o nome de usuário: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = 0;

    char nome_arquivo[150];
    sprintf(nome_arquivo, "%s_senha.txt", nome);

    Criptografar(chave, senha, texto_cript);
    SalvarArquivo(texto_cript, strlen(senha), nome_arquivo);

    printf("Dados do usuário criptografados e salvos com sucesso!\n");
}

void MostrarUsuarios() {
    system("dir /b *_senha.txt");
}

void TrocarSenha(char chave[]) {
    char nome[100], nova_senha[100];
    int texto_cript[100];

    printf("Digite o nome do usuário que deseja trocar a senha: ");
    scanf("%s", nome);
    getchar();

    printf("Digite a nova senha: ");
    fgets(nova_senha, sizeof(nova_senha), stdin);
    nova_senha[strcspn(nova_senha, "\n")] = 0;

    char nome_arquivo[150];
    sprintf(nome_arquivo, "%s_senha.txt", nome);

    Criptografar(chave, nova_senha, texto_cript);
    SalvarArquivo(texto_cript, strlen(nova_senha), nome_arquivo);

    printf("Senha trocada com sucesso!\n");
}

void ExcluirUsuario() {
    char nome[100];
    printf("Digite o nome do usuário que deseja excluir: ");
    scanf("%s", nome);
    getchar();

    char nome_arquivo[150];
    sprintf(nome_arquivo, "%s_senha.txt", nome);

    if (remove(nome_arquivo) == 0) {
        printf("Usuário %s excluído com sucesso!\n", nome);
    } else {
        printf("Erro ao excluir o usuário %s.\n", nome);
    }
}

// Função de autenticação de usuário
int AutenticarUsuario() {
    char username[50], password[50];
    char chave[] = "Abcd";
    int texto_cript[50];
    int i, valor;

    printf("Digite o nome de usuário: ");
    scanf("%s", username);
    getchar();

    printf("Digite a senha: ");
    scanf("%s", password);
    getchar();

    char nome_arquivo[150];
    sprintf(nome_arquivo, "%s_senha.txt", username);

    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("\n\nErro ao abrir o arquivo de senha...\n\n\n");
        return 0;
    }

    int senha_cript[50];
    Criptografar(chave, password, senha_cript);

    i = 0;
    while (fscanf(arquivo, "%d", &valor) == 1 && i < strlen(password)) {
        if (senha_cript[i] != valor) {
            fclose(arquivo);
            printf("\n\nAutenticação falhou! Nome de usuário ou senha incorretos.\n\n\n");
            return 0;
        }
        i++;
    }
    fclose(arquivo);

    printf("\n\nAutenticação bem-sucedida!\n\n\n");
    return 1;
}

int main(void) {
    int opcao = 0;
    char chave[] = "Abcd";

    if (AutenticarUsuario()) {
        do {
            printf("\nSelecione uma opcao: \n 1 - Criptografar texto digitado. \n 2 - Descriptografar texto. \n 3 - Mostrar usuários cadastrados. \n 4 - Trocar senha. \n 5 - Excluir usuário. \n 6 - Sair. \n");
            scanf("%d", &opcao);
            getchar(); // Limpar o buffer de entrada
            switch (opcao) {
            case 1:
                CriptografarTexto(chave);
                break;
            case 2:
                Decriptografar(chave);
                break;
            case 3:
                MostrarUsuarios();
                break;
            case 4:
                TrocarSenha(chave);
                break;
            case 5:
                ExcluirUsuario();
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente. \n\n");
            }
        } while (opcao != 6);
    }

    return 0;
}

