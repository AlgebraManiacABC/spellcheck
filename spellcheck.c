#include "spellcheck.h"

int main()
{
    trie_t dictionary = init_trie();
    if(fill_trie_from_file(&dictionary,"dictionary.txt"))
    {
        printf("Trie creation failure!\n");
        return EXIT_FAILURE;
    }
    //print_trie(dictionary);
    if(exists_in_trie("Test",dictionary))
    {
        printf("Test is in the dictionary!\n");
    }
    else
    {
        printf("Test is not in the dictionary!\n");
    }
    delete_trie(dictionary);
}

node_t create_node()
{
    node_t new = malloc(sizeof(node));
    new->term = false;
    for(int i=0; i<NUM_CHARS; i++)
    {
        new->children[i] = NULL;
    }
    return new;
}

trie_t init_trie()
{
    return create_node();
}

bool insert_word(trie_t * root, const char * to_insert)
{
    if(!to_insert) return false;
    if(!root)
    {
        *root = create_node();
    }

    node_t temp = *root;
    int len = strnlen(to_insert,MAX_LEN);
    char * word = malloc(sizeof(char)*(len+1));
    strncpy(word,to_insert,len+1);
    for(int i=0; i<len; i++)
    {
        word[i] = tolower(word[i]);
        if(!temp->children[(unsigned char)word[i]])
        {
            temp->children[(unsigned char)word[i]] = create_node();
        }
        temp = temp->children[(unsigned char)word[i]];
    }
    free(word);
    if(temp->term)
    {
        return false;
    }
    temp->term = true;
    return true;
}

void print_trie(node_t root)
{
    if(!root) return;
    static char * history = NULL;
    static int len = 0;
    if(!history)
    {
        history = malloc(sizeof(char)*MAX_LEN);
        history[0] = '\0';
    }

    for(int i=0; i<NUM_CHARS; i++)
    {
        history[len] = i;
        history[len+1] = '\0';
        len++;
        print_trie(root->children[i]);
        len--;
    }

    if(root->term)
    {
        printf("%s\n",history);
    }
}

bool exists_in_trie(const char * word, trie_t trie)
{
    if(!word || !trie) return false;
    static char history[256] = {0};
    static int len = 0;

    if(!strcasecmp(word,history))
    {
        return true;
    }
    for(int i=0; i<NUM_CHARS; i++)
    {
        history[len] = i;
        history[len+1] = '\0';
        len++;
        if(exists_in_trie(word,trie->children[i]))
        {
            return true;
        }
        len--;
    }
    return false;
}

int fill_trie_from_file(trie_t * trie, const char * filename)
{
    if(!filename) return EXIT_FAILURE;
    if(!trie) return EXIT_FAILURE;

    FILE * f = fopen(filename,"r");
    if(!f) return EXIT_FAILURE;

    while(f && !feof(f))
    {
        char buffer[MAX_LEN] = {'\0'};
        fgets(buffer,MAX_LEN,f);
        insert_word(trie,buffer);
    }
    fclose(f);
    return EXIT_SUCCESS;
}

void delete_trie(trie_t trie)
{
    if(!trie) return;
    for(int i=0; i<NUM_CHARS; i++)
    {
        delete_trie(trie->children[i]);
    }
    free(trie);
}