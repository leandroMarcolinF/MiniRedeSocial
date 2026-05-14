#include "minirede.h"
#include <cstring>

void inicializarMiniRede(MiniRede& rede) {
    rede.raizArvoreUsuario = nullptr; 
}  

void liberarMiniRede(MiniRede& rede) {
    // TODO
}

void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida) {
    // TODO: ler comandos da entrada padrao ate END.
    // Para cada comando, chamar a funcao correspondente.
    // Nao imprimir menu, prompt ou texto extra.
}

void inserirNaArvore(NoArvoreUsuario*& raiz, const Usuario& usuario, std::ostream& saida) {
    if (raiz == nullptr) {
        raiz = new NoArvoreUsuario{usuario, nullptr, nullptr};
        saida << "USER_ADDED";
        return;
    }
    if (usuario.id == raiz->usuario.id) {
        saida << "ERROR USER_EXISTS";
        return;
    }
    
    if (usuario.id < raiz->usuario.id) {
        inserirNaArvore(raiz->esq, usuario, saida);
    }
    if (usuario.id > raiz->usuario.id) {
        inserirNaArvore(raiz->dir, usuario, saida);
    }
}

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida) {
    Usuario usuarioNovo;
    usuarioNovo.id = id;
    strncpy(usuarioNovo.username, username, TAM_USERNAME - 1);
    usuarioNovo.username[TAM_USERNAME - 1] = '\0';
    strncpy(usuarioNovo.nomeCompleto, nomeCompleto, TAM_NOME - 1);
    usuarioNovo.nomeCompleto[TAM_NOME - 1] = '\0';

    inserirNaArvore(rede.raizArvoreUsuario, usuarioNovo, saida);
}

bool buscarNaArvorePorId(NoArvoreUsuario* no, int id, std::ostream& saida) {
    bool achado = false;

    if (no == nullptr) {
        saida << "ERROR USER_NOT_FOUND";
    }

    if (no->usuario.id == id) {
        saida << "USER" << no->usuario.id << no->usuario.username << no->usuario.nomeCompleto;
    }
    else if (no->usuario.id > id) {
        achado = buscarNaArvorePorId(no->esq, id, saida);
    }
    else if (no->usuario.id < id) {
        achado = buscarNaArvorePorId(no->dir, id, saida);
    }
    return achado;
}

void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida) {
    buscarNaArvorePorId(rede.raizArvoreUsuario, id, saida);
}

void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida) {
    // TODO
}

void listarUsuarios(MiniRede& rede, std::ostream& saida) {
    // TODO
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida) {
    // TODO
}

void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida) {
    // TODO
}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida) {
    // TODO
}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida) {
    // TODO
}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    // TODO
}

void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    // TODO
}

void listarTopPosts(MiniRede& rede, int k, std::ostream& saida) {
    // TODO
}

int main() {
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, std::cin, std::cout);
    liberarMiniRede(rede);

    return 0;
}

