#include "minirede.h"
#include <cstring>
#include <sstream>

void inicializarMiniRede(MiniRede& rede) {
    rede.raizArvoreUsuario = nullptr; 
}

void liberarArvore(NoArvoreUsuario*& raiz) {
    if (raiz == nullptr) {
        return;
    };
    
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    delete raiz;
    raiz = nullptr;
}

void liberarMiniRede(MiniRede& rede) {
    liberarArvore(rede.raizArvoreUsuario);
}

void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida) {
    std::string linha;
    std::string comando;

    while (std::getline(entrada, linha)) {
        std::istringstream stream(linha);
        comando = "";
        stream >> comando;

        if (comando == "END") {
            break;
        } else if (comando == "ADD_USER") {
            int id;
            std::string username, nomeCompleto;
            stream >> id >> username >> nomeCompleto;
            cadastrarUsuario(rede, id, username.c_str(), nomeCompleto.c_str(), saida);
        } else if (comando == "FIND_USER") {
            int id;
            stream >> id;
            buscarUsuarioPorId(rede, id, saida);
        } else if (comando == "LIST_USERS") {
            listarUsuarios(rede, saida);
        } else {
            saida << "ERROR INVALID_COMMAND" << std::endl;
        }
    }
}

void inserirNaArvore(NoArvoreUsuario*& raiz, const Usuario& usuario, std::ostream& saida) {
    if (raiz == nullptr) {
        raiz = new NoArvoreUsuario{usuario, nullptr, nullptr};
        saida << "USER_ADDED" << std::endl;
        return;
    }
    if (usuario.id == raiz->usuario.id) {
        saida << "ERROR USER_EXISTS" << std::endl; 
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

void buscarNaArvorePorId(NoArvoreUsuario* raiz, int id, std::ostream& saida) {
    if (raiz == nullptr) {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }

    if (raiz->usuario.id == id) {
        saida << "USER "
            << raiz->usuario.id << " "
            << raiz->usuario.username << " "
            << raiz->usuario.nomeCompleto
            << std::endl;
    }
    else if (raiz->usuario.id > id) {
        buscarNaArvorePorId(raiz->esq, id, saida);
    }
    else if (raiz->usuario.id < id) {
        buscarNaArvorePorId(raiz->dir, id, saida);
    }
}

void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida) {
    buscarNaArvorePorId(rede.raizArvoreUsuario, id, saida);
}

bool buscarNaArvorePorUsername(NoArvoreUsuario* raiz, const char username[], std::ostream& saida) {
    bool achado = false;

    if (achado == true) {
        return achado;
    }
    
    if (raiz == nullptr) {
        return achado;
    };
    
    if (raiz->usuario.username == username) {
        saida << "USER "
            << raiz->usuario.id << " "
            << raiz->usuario.username << " "
            << raiz->usuario.nomeCompleto
            << std::endl;
        return true;
    }
    buscarNaArvorePorUsername(raiz->esq, username, saida);
    buscarNaArvorePorUsername(raiz->esq, username, saida);

    return achado;
}

void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida) {
    if (!buscarNaArvorePorUsername(rede.raizArvoreUsuario, username, saida)) {
        saida << "ERROR USER_NOT_FOUND" << std::endl; 
    }
}

void listarUsuariosEmOrdemId(NoArvoreUsuario* raiz, std::ostream& saida ) {
    if (raiz == nullptr) {
        return;
    };
    
    listarUsuariosEmOrdemId(raiz->esq, saida);
    saida << "USER "
        << raiz->usuario.id << " "
        << raiz->usuario.username << " "
        << raiz->usuario.nomeCompleto
        << std::endl;
    listarUsuariosEmOrdemId(raiz->dir, saida);
}

void listarUsuarios(MiniRede& rede, std::ostream& saida) {
    listarUsuariosEmOrdemId(rede.raizArvoreUsuario, saida);
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

