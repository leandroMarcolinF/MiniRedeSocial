#include "minirede.h"
#include <cstring>
#include <new>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>

void inicializarMiniRede(MiniRede& rede) {
    rede.raizArvoreUsuario = nullptr;
    for (int i = 0; i < TAM_HASH; i++) {
        rede.tabelaHashUsuario[i] = nullptr;
    }
    rede.listaPublicacoes.inicio = nullptr;
}

void liberarArvore(NoArvoreUsuario*& raiz) {
    if (raiz == nullptr) {
        return;
    };
    
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    delete raiz->usuario;
    delete raiz;
    raiz = nullptr;
}

void liberarHash(MiniRede& rede) {
    for (int i = 0; i < TAM_HASH; i++) {
        NoHashUsuario* atual = rede.tabelaHashUsuario[i];
        while (atual != nullptr) {
            NoHashUsuario* proximo = atual->proximo;
            delete atual;
            atual = proximo;
        }
        rede.tabelaHashUsuario[i] = nullptr;
}
}

void liberarMiniRede(MiniRede& rede) {
    liberarArvore(rede.raizArvoreUsuario);
    liberarHash(rede);
}

/*void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida) {
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
        } else if (comando == "FIND_USERNAME") {
            std::string username;
            stream >> username;
            buscarUsuarioPorUsername(rede, username.c_str(), saida);
        } else if (comando == "FOLLOW") {
            int idSeguidor, idSeguido;
            stream >> idSeguidor >> idSeguido;
            seguirUsuario(rede, idSeguidor, idSeguido, saida);
        } else if (comando == "LIST_FOLLOWING") {
            int id;
            stream >> id;
            listarSeguindo(rede, id, saida);
        } else {
            saida << "ERROR INVALID_COMMAND" << std::endl;
        }
    }
}*/

void processarComandos(MiniRede& rede, std::ostream& saida) {
    char* input;

    while ((input = readline("> ")) != nullptr) {
        std::string linha(input);

        if (!linha.empty()) {
            add_history(input);
        }

        free(input);

        std::istringstream stream(linha);

        std::string comando;
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

        } else if (comando == "FIND_USERNAME") {
            std::string username;
            stream >> username;
            buscarUsuarioPorUsername(rede, username.c_str(), saida);

        } else if (comando == "FOLLOW") {
            int idSeguidor, idSeguido;
            stream >> idSeguidor >> idSeguido;
            seguirUsuario(rede, idSeguidor, idSeguido, saida);

        } else if (comando == "LIST_FOLLOWING") {
            int id;
            stream >> id;
            listarSeguindo(rede, id, saida);

        } else if (comando == "ADD_POST") {
            int idPost, idAutor, timestamp;
            std::string texto;
            stream >> idPost >> idAutor >> timestamp >> texto;
            cadastrarPublicacao(rede, idPost, idAutor, timestamp, texto.c_str(), saida);

        } else {
            saida << "ERROR INVALID_COMMAND" << std::endl;
        }
    }
}

Usuario* inserirNaArvore(NoArvoreUsuario*& raiz, const Usuario& usuario, std::ostream& saida) {
    if (raiz == nullptr) {
        Usuario* novoUsuario = new Usuario(usuario);
        raiz = new NoArvoreUsuario{novoUsuario, nullptr, nullptr};
        saida << "USER_ADDED" << std::endl;
        return novoUsuario;
    }
    if (usuario.id == raiz->usuario->id) {
        saida << "ERROR USER_EXISTS" << std::endl;
        return nullptr;
    }

    if (usuario.id < raiz->usuario->id) {
        return inserirNaArvore(raiz->esq, usuario, saida);
    }

    return inserirNaArvore(raiz->dir, usuario, saida);
}

int funcaoHash(const char username[]) {
    unsigned int chave = 0;

    for (int pos = 0; username[pos] != '\0'; pos++) {
        chave = chave * 31 + username[pos];
    }

    return chave % TAM_HASH;
}

void inserirNoHash(MiniRede& rede, Usuario* usuario) {
    int indice = funcaoHash(usuario->username);
    
    NoHashUsuario* noHash = new NoHashUsuario;
    noHash->usuario = usuario;
    noHash->proximo = rede.tabelaHashUsuario[indice];
    rede.tabelaHashUsuario[indice] = noHash;
}

void cadastrarUsuario(MiniRede& rede, int id, const char username[], const char nomeCompleto[], std::ostream& saida) {
    Usuario usuarioNovo;
    usuarioNovo.id = id;
    strncpy(usuarioNovo.username, username, TAM_USERNAME - 1);
    usuarioNovo.username[TAM_USERNAME - 1] = '\0';
    strncpy(usuarioNovo.nomeCompleto, nomeCompleto, TAM_NOME - 1);
    usuarioNovo.nomeCompleto[TAM_NOME - 1] = '\0';
    usuarioNovo.listaSeguidores.inicio = nullptr;

    Usuario* inserido = inserirNaArvore(rede.raizArvoreUsuario, usuarioNovo, saida);
    if (inserido != nullptr) {
        inserirNoHash(rede, inserido);
    }
}

Usuario* buscarNaArvorePorId(NoArvoreUsuario* raiz, int id) {
    if (raiz == nullptr) {
        return nullptr;
    }

    if (raiz->usuario->id == id) {
        return raiz->usuario;
    }
    if (id < raiz->usuario->id) {
        return buscarNaArvorePorId(raiz->esq, id);
    } 
    return buscarNaArvorePorId(raiz->dir, id);
}

void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida) {
    Usuario* usuarioAchado = buscarNaArvorePorId(rede.raizArvoreUsuario, id);
    if (usuarioAchado == nullptr) {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }

    saida << "USER "
        << usuarioAchado->id << " "
        << usuarioAchado->username << " "
        << usuarioAchado->nomeCompleto
        << std::endl;
}

void buscarNoHash(MiniRede& rede, const char username[], std::ostream& saida) {
    int indice = funcaoHash(username);
    NoHashUsuario* atual = rede.tabelaHashUsuario[indice];

    while (atual != nullptr) {
        if (strncmp(atual->usuario->username, username, TAM_USERNAME) == 0) {
            saida << "USER "
                << atual->usuario->id << " "
                << atual->usuario->username << " "
                << atual->usuario->nomeCompleto
                << std::endl;
            return;
        }
        atual = atual->proximo;
    }

    saida << "ERROR USER_NOT_FOUND" << std::endl;
}

void buscarUsuarioPorUsername(MiniRede& rede, const char username[], std::ostream& saida) {
    buscarNoHash(rede, username, saida);
}

void listarUsuariosEmOrdemId(NoArvoreUsuario* raiz, std::ostream& saida ) {
    if (raiz == nullptr) {
        return;
    };
    
    listarUsuariosEmOrdemId(raiz->esq, saida);
    saida << "USER "
        << raiz->usuario->id << " "
        << raiz->usuario->username << " "
        << raiz->usuario->nomeCompleto
        << std::endl;
    listarUsuariosEmOrdemId(raiz->dir, saida);
}

void listarUsuarios(MiniRede& rede, std::ostream& saida) {
    listarUsuariosEmOrdemId(rede.raizArvoreUsuario, saida);
}

bool inserirSeguidorOrdenado(ListaSeguidores &L, int id) {
    NoSeguidor* atual = L.inicio;
    NoSeguidor* anterior = nullptr;

    while (atual != nullptr && atual->id <= id) {
        if (atual->id == id) {
            return false;
        }
        anterior = atual;
        atual = atual->prox;
    }

    NoSeguidor* no = new NoSeguidor{id, atual};
    if (anterior == nullptr) {
        L.inicio = no;
    } else {
        anterior->prox = no;
    }

    return true;
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida) {
    if (idSeguidor == idSeguido) {
        saida << "ERROR CANNOT_FOLLOW_SELF" << std::endl;
        return;
    }

    Usuario* seguidor = buscarNaArvorePorId(rede.raizArvoreUsuario, idSeguidor);
    Usuario* seguido  = buscarNaArvorePorId(rede.raizArvoreUsuario, idSeguido);

    if (seguidor == nullptr || seguido == nullptr) {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }

    if (!inserirSeguidorOrdenado(seguidor->listaSeguidores, idSeguido)) {
        saida << "ERROR ALREADY_FOLLOWING" << std::endl;
        return;
    }

    saida << "FOLLOWED" << std::endl;
}

void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida) {
    Usuario* usuarioAchado = buscarNaArvorePorId(rede.raizArvoreUsuario, idUsuario);
    if (usuarioAchado == nullptr) {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }

    NoSeguidor* atual = usuarioAchado->listaSeguidores.inicio;

    saida << "FOLLOWING_BEGIN" << std::endl;
    while (atual != nullptr) {
        Usuario* seguido = buscarNaArvorePorId(rede.raizArvoreUsuario, atual->id);
        if (seguido != nullptr) {
            saida << "USER "
                << seguido->id << " "
                << seguido->username << " " 
                << seguido->nomeCompleto
                << std::endl;
        }
        atual = atual->prox;
    }
    saida << "FOLLOWING_END" << std::endl;
}

bool inserirPublicacaoOrdenado(ListaPublicacoes &L, int id, int idAutor, int timestamp, const char texto[]) {
    NoPublicacao* no = new NoPublicacao{id , idAutor,timestamp, nullptr, texto};
    no->listaCurtidasPost.inicio = nullptr; //lógica de curtida
    NoPublicacao* atual = L.inicio;
    NoPublicacao* anterior = nullptr;

    while (atual != nullptr && atual->id <= id) {
        if (atual->id == id) {
            return false;
        }
        anterior = atual;
        atual = atual->prox;
    }

    no->prox = atual;
    if (anterior == nullptr) {
        L.inicio = no;
    } else {
        anterior->prox = no;
    }

    return true;

}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida) {

    Usuario* usuarioAchado = buscarNaArvorePorId(rede.raizArvoreUsuario, idAutor);
    if (usuarioAchado == nullptr) {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }

    if(!inserirPublicacaoOrdenado(rede.listaPublicacoes, idPost, idAutor, timestamp, texto)) {
        saida << "ERROR POST_EXISTS" << std::endl;
    }

    saida << "POST_ADDED" << std::endl;

}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida) {
    
    NoCurtida* no = new NoCurtida{idUsuario};

    // como caralhos eu acho o id da publicação sem ter que varrer todas as publicações? kkkkkkkkk fudeu
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
    //processarComandos(rede, std::cin, std::cout);
    processarComandos(rede, std::cout);
    liberarMiniRede(rede);

    return 0;
}

