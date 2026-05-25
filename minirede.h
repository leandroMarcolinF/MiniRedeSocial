#ifndef MINI_REDE_H
#define MINI_REDE_H

#include <iostream>

const int TAM_USERNAME = 50;
const int TAM_NOME = 100;
const int TAM_TEXTO = 280;
const int TAM_COMANDO = 30;
const int TAM_HASH = 101;

struct NoSeguidor {
    int id;
    NoSeguidor* prox;
};

struct ListaSeguidores {
    NoSeguidor* inicio;
};

struct NoPublicacao {
    int id;
    int timestamp;
    NoPublicacao* prox;
    std::string texto;
};

struct ListaPublicacoes {
    NoPublicacao* inicio;
};

struct Usuario {
    int id;
    char username[TAM_USERNAME];
    char nomeCompleto[TAM_NOME];
    ListaSeguidores listaSeguidores;
    ListaPublicacoes listaPublicacoes;
};
struct NoArvoreUsuario {
    Usuario* usuario;
    NoArvoreUsuario* esq;
    NoArvoreUsuario* dir;
};

struct NoHashUsuario {
    Usuario* usuario;
    NoHashUsuario* proximo;
};

struct MiniRede {
    NoArvoreUsuario* raizArvoreUsuario;
    NoHashUsuario* tabelaHashUsuario[TAM_HASH];
    // - publicacoes cadastradas
};

void inicializarMiniRede(MiniRede& rede);
void liberarMiniRede(MiniRede& rede);
//void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida);
void processarComandos(MiniRede& rede, std::ostream& saida);

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida);
void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida);
void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida);
void listarUsuarios(MiniRede& rede, std::ostream& saida);

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida);
void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida);

bool inserirPublicacaoOrdenado(ListaPublicacoes &L, int id, int timestamp, char texto[]);
void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida);
void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida);

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void listarTopPosts(MiniRede& rede, int k, std::ostream& saida);

void liberarArvore(NoArvoreUsuario*& raiz);
Usuario* inserirNaArvore(NoArvoreUsuario*& raiz, const Usuario& usuario, std::ostream& saida);
void buscarNaArvorePorId(NoArvoreUsuario* raiz, int id, std::ostream& saida);

void buscarNoHash(MiniRede& rede, const char username[], std::ostream& saida);
void listarUsuariosEmOrdemId(NoArvoreUsuario* raiz, std::ostream& saida);

#endif
