#include "cream.h"
#include "utils.h"
#include <stdio.h>
#include "queue.h"
#include "hashmap.h"
#include <string.h>

int main(int argc, char *argv[]) {
    queue_t *queue = create_queue();
    if(queue == NULL) {
        printf("%s\n", "CREATE QUEUE FAILED");
    }
    else {
        printf("%s\n", "CREATE QUEUE SUCCESS");
    }
    int num = 10;
    void *item = &num;
    bool isEnqueue = enqueue(queue, item);
    printf("ENQUEUE IS : %d\n", isEnqueue);

    void *removedItem = dequeue(queue);
    printf("DEQUEUED VALUE IS : %d\n", *((int*)removedItem));

    int num2 = 20;
    void *item2 = &num2;
    isEnqueue = enqueue(queue, item2);
    printf("ENQUEUE IS : %d\n", isEnqueue);

    int num3 = 30;
    void *item3 = &num3;
    isEnqueue = enqueue(queue, item3);
    printf("ENQUEUE IS : %d\n", isEnqueue);


    void *removedItem2 = dequeue(queue);
    printf("DEQUEUED VALUE IS : %d\n", *((int*)removedItem2));
    void *removedItem3 = dequeue(queue);
    printf("DEQUEUED VALUE IS : %d\n", *((int*)removedItem3));

    printf("=======================HASHMAP========================\n");

    //destroy function can not be decleared manually by user,

    //hashmap_t *map = create_map(10, )
    //int mapSize = sizeof(hashmap_t);
    //printf("SIZE OF A HASHMAP : %d\n", mapSize);

    // char *a = "abcd";
    // char *b = "abcdefg";
    // printf("%d\n", memcmp(a, b, 4));

    exit(0);
}
