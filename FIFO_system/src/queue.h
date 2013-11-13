/*
 *	queue.h
 *
 *	Victor Oudin & Azibi Mounia
 *
 */ 

 /* Ce fichier définit les interfaces que vous devez implémenter. */

/* Le C est bizarre comme-ça, il aime bien qu'on lui mentionne les
   structures dont on se sert même si on ne les définit pas. */
#include <pthread.h>

struct queue;
struct iovec;

/* Création et destruction. */

struct queue *queue_new(size_t size);
void queue_destroy(struct queue *queue);

/* Enfilage et défilage. */

int enqueue(struct queue *queue, const void *buf, size_t size, int blocking);
ssize_t dequeue(struct queue *queue, void *buf, size_t bufsize, int blocking);

/* Enfilage et défilage multiples. */

int enqueue_multiple(struct queue *queue, const struct iovec *iov, int iovcnt,
                     int blocking);
int dequeue_multiple(struct queue *queue, struct iovec *iov, int iovcnt,
                     int blocking);

/* Enfilage multiple dynamique. */
int enqueue_multiple_dynamic(struct queue *queue, const struct iovec *iov, int iovcnt);
