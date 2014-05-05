/*******************************************************************************
 * Carona Comunitária USP está licenciado com uma Licença Creative Commons
 * Atribuição-NãoComercial-CompartilhaIgual 4.0 Internacional (CC BY-NC-SA 4.0).
 * 
 * Carona Comunitária USP is licensed under a Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License (CC BY-NC-SA 4.0).
 * 
 * Tratamento de conexões e threads
*******************************************************************************/
#ifndef __CONEXAO__H__
#define __CONEXAO__H__

#include "global.h"
#include "hash.h"
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>

#define th_error(msg)		do {fprintf(stderr, "Thread %d: %s error - %s\n", tsd->n_thread, msg, strerror(errno)); pthread_exit(NULL);} while(0)
#define th_try(cmd,msg)		do {if ((cmd) == -1) {th_error(msg);}} while(0)
#define th_try0(cmd,msg)	do {if ((cmd) == NULL) {th_error(msg);}} while(0)
#define th_tryEOF(cmd,msg)	do {if ((cmd) == EOF) {th_error(msg);}} while(0)

extern int s, clientes_agora, clientes_total, caronas_total;
extern uint32_t conectados[MAX_CLIENTES];	// lista de IPs já conectados
											///@WARN: 32 bits para IPv4 apenas
extern pthread_mutex_t mutex_modifica_thread;
extern pthread_cond_t comunica_thread[MAX_CLIENTES];
extern pthread_key_t dados_thread;
extern int pilha_threads_livres[MAX_CLIENTES];

// Thread Specific Data Area - regiões alocadas para cada thread para
// guardar variáveis da thread (se usássemos globais, haveria conflito entre
// as threads). Ver pthread_key_create(3)
typedef struct {
	int fd_con;		// file descriptor da conexão
	int n_thread;	// número da thread
	char *errmsg;
} tsd_t;

typedef struct {
	// Início do buffer de leitura, fim da mensagem atual, fim do pacote atual
	// e tamanho máximo do buffer
	char *area, *fim_msg, *fim_pacote;
	size_t tamanho_area;
} leitura_t;

inline int ja_conectado(const struct in_addr *ip);
inline void aceita_conexao(tsd_t *tsd, const struct in_addr *ip);
void* th_conecao_cliente(void *tmp);

__attribute__((noreturn))void pthread_exit(void *retval);

#endif
