#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define NUM_CHARS 256
#define MAX_LEN 512

typedef struct node
{
    bool term;
    struct node * children[NUM_CHARS];
}   node;

typedef node * node_t;
typedef node_t trie_t;

node_t create_node();
trie_t init_trie();

bool insert_word(node_t * root, const char * word);
void print_trie(node_t root);
bool exists_in_trie(const char * word, trie_t trie);

int fill_trie_from_file(trie_t * trie, const char * filename);

void delete_trie(trie_t trie);