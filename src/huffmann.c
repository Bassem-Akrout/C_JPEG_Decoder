#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct huffnode {
    int D;
    char c[17];
    struct huffnode* left;
    struct huffnode* right;
    struct huffnode* P;
} huffnode;

huffnode* create_huffnode(huffnode* parent, char* char_value) {
    huffnode* new_node = (huffnode*)malloc(sizeof(huffnode));
    if (parent == NULL) {
        new_node->D = 0;
        strcpy(new_node->c, "");
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->P = NULL;
    } else {
        new_node->D = parent->D + 1;
        strcpy(new_node->c, parent->c);
        strcat(new_node->c, char_value);
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->P = parent;
    }
    return new_node;
}

huffnode* goleft(huffnode* parent, char* char_value) {
    huffnode* left_child = create_huffnode(parent, char_value);
    parent->left = left_child;
    return left_child;
}

huffnode* goright(huffnode* parent, char* char_value) {
    huffnode* right_child = create_huffnode(parent, char_value);
    parent->right = right_child;
    return right_child;
}

huffnode* find_free_parent(huffnode* node) {
    if (node->right == NULL && node->left != NULL) {
        return node;
    }
    if (node->P != NULL) {
        node = node->P;
        while (node->right != NULL) {
            node = node->P;
        }
    }
    return node;
}

unsigned char** huffmancodes(uint8_t* list_length, huffnode* root, int list_size) {
    unsigned char** list_codes = (unsigned char**)malloc(list_size * sizeof(unsigned char*)); 
    for (int i = 0; i < list_size; i++) {
        list_codes[i] = (unsigned char*)calloc(17 , sizeof(unsigned char));
    }
    int j=0;
    uint8_t list_length2[16];
    for (int i = 0; i < 16; i++){
        list_length2[i]=list_length[i];
    }
    for (int i = 0; i < 16; i++) {     
        while (list_length2[i] > 0) {
            root = find_free_parent(root);
            while (root->D < i + 1) {
                if (root->left == NULL) {
                    root = goleft(root, "0");
                } else {
                    root = goright(root, "1");
                }
            }
            list_length2[i] -= 1;
            strcpy(list_codes[j], root->c);
            j+=1;
            root = root->P;
            if (list_length2[i] > 0) {
                root = goright(root, "1");
                list_length2[i] -= 1;
                strcpy(list_codes[j], root->c);
                j+=1;
            }
        
        }
    }   
    return list_codes;
}

void free_hufftree(huffnode* node) {
    if (node == NULL) {
        return;
    }
    free_hufftree(node->left);
    free_hufftree(node->right);
    free(node);
}


void free_huffmancodes(unsigned char** list_codes, int list_size) {
    for (int i = 0; i < list_size; i++) {
        free(list_codes[i]);
    }
    free(list_codes);
}

/*int main(void){
    uint8_t frequencies[16] = {0 ,1 ,3 ,3 ,2 ,3 ,5 ,5 ,6 ,5 ,2 ,3 ,4 ,7 ,4 ,11};
    huffnode* root = create_huffnode(NULL, "");
    char** codes = huffmancodes(frequencies, root, 64);

    // Print the Huffman codes
    for (int j = 0; j < 64; j++){
        printf("%s\n", codes[j]);
    } 
    

    // Free dynamically allocated memory
    for (int i = 0; i < 64; i++) {
        free(codes[i]);
    }
    free(codes);

    return 0;
}*/
/*int main(void){
    uint8_t frequencies[16] = {0 ,0 ,7 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0,0};
    huffnode* root = create_huffnode(NULL, "");
    char** codes = huffmancodes(frequencies, root, 10);

    // Print the Huffman codes
    for (int j = 0; j < 10; j++){
        printf("%s\n", codes[j]);
    } 
    

    // Free dynamically allocated memory
    for (int i = 0; i < 10; i++) {
        free(codes[i]);
    }
    free(codes);

    return 0;
}*/