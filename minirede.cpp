#include "minirede.h"
//#include <cstdio>
#include <cstring>
#include <new>
#include <readline/chardefs.h>
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
        
        } else if (comando == "LIKE") {
            int idAutor, idPost;
            stream >> idAutor >> idPost;
            curtirPublicacao(rede, idAutor, idPost, saida);

        } else if (comando == "GET_NOTIFICATIONS") {
            int idUsuario, k;
            stream >> idUsuario >> k;
            consultarNotificacoes(rede, idUsuario, k, saida);

        } else if (comando == "FEED") {
            int idUsuario, k;
            stream >> idUsuario >> k;
            gerarFeed(rede, idUsuario, k, saida);

        } else if (comando == "TOP_POSTS") {
            int k;
            stream >> k;
            listarTopPosts(rede, k, saida);

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
    usuarioNovo.filaNotificacao.inicio = nullptr;
    usuarioNovo.filaNotificacao.fim = nullptr;

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

void inserirNotificacao(MiniRede& rede, int idUsuarioNotificador, int idUsuarioNotificado, int idPostOrFollow) {

    Usuario* usuario  = buscarNaArvorePorId(rede.raizArvoreUsuario, idUsuarioNotificado);
    noNotificacao* no = new noNotificacao{idUsuarioNotificador , idPostOrFollow, nullptr};

    if(usuario->filaNotificacao.inicio == nullptr){
        usuario->filaNotificacao.inicio = no;
        usuario->filaNotificacao.fim = no;
    }
    else{
        no->prox = usuario->filaNotificacao.fim;
        usuario->filaNotificacao.inicio = no;
    }
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

    inserirNotificacao(rede, seguidor->id, seguido->id, -1);

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

bool inserirPublicacao(ListaPublicacoes &L, int id, int idAutor, int timestamp, const char texto[]) {
    NoPublicacao* no = new NoPublicacao{id, idAutor, timestamp, 0,nullptr, texto};
    no->listaCurtidasPost.inicio = nullptr; //lógica de curtida

    NoPublicacao* atual = L.inicio;
    NoPublicacao* anterior = nullptr;

    while (atual != nullptr) {
        if (atual->id == id) {
            return false;
        }
        anterior = atual;
        atual = atual->prox;
    }

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

    if(!inserirPublicacao(rede.listaPublicacoes, idPost, idAutor, timestamp, texto)) {
        saida << "ERROR POST_EXISTS" << std::endl;
        return;
    }

    saida << "POST_ADDED" << std::endl;

}

NoPublicacao* encontrarPublicacao(MiniRede& rede, ListaPublicacoes &L, int idPost){

    NoPublicacao* atual = L.inicio;

    while (atual != nullptr) {
        if (atual->id == idPost) {
            return atual;
        }
        atual = atual->prox;
    }

    return nullptr;
}

NoCurtida* encontrarCurtida(MiniRede& rede, ListaCurtidasPost &L, int idUsuario){
    NoCurtida* atual = L.inicio;

    while (atual != nullptr && atual->idUsuario <= idUsuario) {
        if (atual->idUsuario == idUsuario) {
            return atual;
        }
        atual = atual->prox;
    }

    return nullptr;

}

bool inserirCurtidaOrdenado(ListaCurtidasPost &L, int idUsuario) {
    NoCurtida* atual = L.inicio;
    NoCurtida* anterior = nullptr;

    while (atual != nullptr && atual->idUsuario <= idUsuario) {
        if (atual->idUsuario == idUsuario) {
            return false;
        }
        anterior = atual;
        atual = atual->prox;
    }

    NoCurtida* no = new NoCurtida{idUsuario};
    no->prox = atual;
    if (anterior == nullptr) {
        L.inicio = no;
    } else {
        anterior->prox = no;
    }

    return true;
}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida) {

    Usuario* usuarioAchado = buscarNaArvorePorId(rede.raizArvoreUsuario, idUsuario);
    if (usuarioAchado == nullptr) {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }

    NoPublicacao* publicacaoCurtida = encontrarPublicacao(rede, rede.listaPublicacoes, idPost);
    if (publicacaoCurtida == nullptr) {
        saida << "ERROR POST_NOT_FOUND" << std::endl;
        return;
    }
    
    if (!inserirCurtidaOrdenado(publicacaoCurtida->listaCurtidasPost, idUsuario)) {
        saida << "ERROR ALREADY_LIKED" << std::endl;
        return;
    }

    publicacaoCurtida->curtidas++;
    inserirNotificacao(rede, idUsuario, publicacaoCurtida->idAutor, idPost);

    saida << "LIKED" << std::endl;
    // TODO
}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {
    Usuario* usuario = buscarNaArvorePorId(rede.raizArvoreUsuario, idUsuario);
    if (usuario == nullptr) {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }

    saida << "NOTIFICATIONS_BEGIN" << std::endl;

    for (int i = 0; i < k; i++) {
        if (usuario->filaNotificacao.inicio == nullptr) break;

        noNotificacao* atual = usuario->filaNotificacao.inicio;
        noNotificacao* anterior = nullptr;

        while (atual->prox != nullptr) {
            anterior = atual;
            atual = atual->prox;
        }

        if (atual->isFollowNot == -1) {
            saida << "NOTIFICATION FOLLOW " << atual->idUsuarioNotificador << std::endl;
        } else {
            saida << "NOTIFICATION LIKE " << atual->idUsuarioNotificador << " " << atual->isFollowNot << std::endl;
        }

        if (anterior == nullptr) {
            usuario->filaNotificacao.inicio = nullptr;
            usuario->filaNotificacao.fim = nullptr;
        } else {
            anterior->prox = nullptr;
            usuario->filaNotificacao.fim = anterior;
        }

        delete atual;
    }

    saida << "NOTIFICATIONS_END" << std::endl;
}

void insertionSortCurtidas(ListaPublicacoes &L){
    NoPublicacao* ordenada = nullptr;

    while (L.inicio != nullptr) {
        NoPublicacao* atual = L.inicio;
        L.inicio = L.inicio->prox;

        if (ordenada == nullptr ||
            atual->curtidas > ordenada->curtidas ||
            (atual->curtidas == ordenada->curtidas &&
             atual->id < ordenada->id))
        {
            atual->prox = ordenada;
            ordenada = atual;
        }
        else {
            NoPublicacao* p = ordenada;

            while (p->prox != nullptr &&
                   (
                       p->prox->curtidas > atual->curtidas ||
                       (
                           p->prox->curtidas == atual->curtidas &&
                           p->prox->id < atual->id
                       )
                   ))
            {
                p = p->prox;
            }

            atual->prox = p->prox;
            p->prox = atual;
        }
    }

    L.inicio = ordenada;
}

void insertionSortTimestamp(ListaPublicacoes &L){
    NoPublicacao* ordenada = nullptr;

    while (L.inicio != nullptr) {
        NoPublicacao* atual = L.inicio;
        L.inicio = L.inicio->prox;

        if (ordenada == nullptr ||
            atual->timestamp > ordenada->timestamp ||
            (atual->timestamp == ordenada->timestamp &&
             atual->id < ordenada->id))
        {
            atual->prox = ordenada;
            ordenada = atual;
        }
        else {
            NoPublicacao* p = ordenada;

            while (p->prox != nullptr &&
                   (
                       p->prox->timestamp > atual->timestamp ||
                       (
                           p->prox->timestamp == atual->timestamp &&
                           p->prox->id < atual->id
                       )
                   ))
            {
                p = p->prox;
            }

            atual->prox = p->prox;
            p->prox = atual;
        }
    }

    L.inicio = ordenada;
}

void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida) {

    insertionSortTimestamp(rede.listaPublicacoes);

    Usuario* usuarioAchado = buscarNaArvorePorId(rede.raizArvoreUsuario, idUsuario);
    if (usuarioAchado == nullptr) {
        saida << "ERROR USER_NOT_FOUND" << std::endl;
        return;
    }

    NoPublicacao* publicacaoAtual = rede.listaPublicacoes.inicio;

    int i = 0;

    saida << "FEED_BEGIN" << std::endl;

    while(publicacaoAtual != nullptr && i < k){

        NoSeguidor* seguidorAtual = usuarioAchado->listaSeguidores.inicio;
        
        while(seguidorAtual != nullptr){
            if (seguidorAtual->id == publicacaoAtual->idAutor) {
                saida << "POST " << publicacaoAtual->id << " " << publicacaoAtual->idAutor << " " << publicacaoAtual->timestamp << " " << publicacaoAtual->curtidas << " " << publicacaoAtual->texto.c_str() << std::endl;
                i++;
                break;
            }
            seguidorAtual = seguidorAtual->prox;
        }
        publicacaoAtual = publicacaoAtual->prox;
    }

    saida << "FEED_END" << std::endl;
}

void listarTopPosts(MiniRede& rede, int k, std::ostream& saida) {

    insertionSortCurtidas(rede.listaPublicacoes);

    NoPublicacao* publicacaoAtual = rede.listaPublicacoes.inicio;

    int i = 0;

    saida << "TOP_POSTS_BEGIN" << std::endl;

    while(publicacaoAtual != nullptr && i < k){
        saida << "POST " << publicacaoAtual->id << " " << publicacaoAtual->idAutor << " " << publicacaoAtual->timestamp << " " << publicacaoAtual->curtidas << " " << publicacaoAtual->texto.c_str() << std::endl;
        i++;
        publicacaoAtual = publicacaoAtual->prox;
    }

    saida << "TOP_POSTS_END" << std::endl;
}

int main() {
    MiniRede rede;

    inicializarMiniRede(rede);
    //processarComandos(rede, std::cin, std::cout);
    processarComandos(rede, std::cout);
    liberarMiniRede(rede);

    return 0;
}