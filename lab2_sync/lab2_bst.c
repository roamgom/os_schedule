/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"

pthread_mutex_t thread_lock_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thread_lock_2 = PTHREAD_MUTEX_INITIALIZER;

/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */

int get_node(lab2_node* node){
    if (node){
        // reflection to search every node in BST
        get_node(node->left);
        printf("%d ", node->key);
        get_node(node->right);

        // free node memory
        node = NULL;
        free(node);
    }
    return 1;
}

int lab2_node_print_inorder(lab2_tree *tree) {
    // call reflection function to print all node
    int node_status = get_node(tree->root);
    return node_status;
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create() {
    // Allocate lab2_tree
    lab2_tree* tree = (lab2_tree*)malloc(sizeof(lab2_tree));
    tree->root = NULL;
    return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node * lab2_node_create(int key) {
    // Allocate labe2_node with key value
    lab2_node* node = (lab2_node*)malloc(sizeof(lab2_node));
    node -> key = key;
    node -> left = NULL;
    node -> right = NULL;
    return node;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node){
    // Insert node into tree
    // No lock applied
    if (tree->root == NULL){
        tree->root = new_node;
        return 1;
    }

    lab2_node* root_search = tree->root;
    lab2_node* tmp = NULL;

    while (root_search){
        // search the key value if it's in the tree
        tmp = root_search;
        if (new_node->key == root_search->key){
            // delete node if the key is same with previous one
            lab2_node_delete(new_node);
            return -1;
        }
        if (new_node->key < root_search->key)
            root_search = root_search->left;
        else
            root_search = root_search->right;
    }
    if (new_node->key < tmp->key)
        tmp->left = new_node;
    else
        tmp->right = new_node;
    return 1;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node){
    // Insert node into tree
    // with fine-grained lock applied

    pthread_mutex_lock(&thread_lock_1);
    // lock thread to set value to root
    if (tree->root == NULL){
        // add new_node through root
        tree->root = new_node;
        // unlock after adding new_node
        pthread_mutex_unlock(&thread_lock_1);
        return 1;
    }
    // unlock thread after adding node to tree
    pthread_mutex_unlock(&thread_lock_1);

    // set check_node to find the proper location to add node
    lab2_node* check_root_node = tree->root;
    lab2_node* tmp_node = NULL;

    while(1){
        tmp_node = check_root_node;
        if (new_node->key == check_root_node->key){
            // delete node and quit if the key value already exsits
            lab2_node_delete(new_node);
            return -1;
        }
        // lock before locating new_node
        // 여기 락부분 (형코드)
        if (new_node->key < check_root_node->key)
            // move to left if the new_node value is smaller
            check_root_node = check_root_node->left;
        else
            // move to right if the new_node value is larger
            check_root_node = check_root_node->right;
        
        pthread_mutex_lock(&tmp_node->mutex);
        if (check_root_node == NULL)
            break;
        pthread_mutex_unlock(&tmp_node->mutex);
    }
    if (new_node->key < tmp_node->key)
        tmp_node->left = new_node;
    else
        tmp_node->right = new_node;

    // unlock after inserting node
    pthread_mutex_unlock(&tmp_node->mutex);
    return 1;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node){

    // lock before adding new_node
    pthread_mutex_lock(&thread_lock_1);
    if (tree->root == NULL){
        // add new_node through root
        tree->root = new_node;
        // unlock after adding new_node
        pthread_mutex_unlock(&thread_lock_1);
        return 1;
    }

    lab2_node* check_root_node = tree->root;
    lab2_node* tmp_node = NULL;

    while(check_root_node){
        tmp_node = check_root_node;
        if (new_node->key == check_root_node->key){
            // delete node and quit if the key value already exsits
            lab2_node_delete(new_node);
            pthread_mutex_unlock(&thread_lock_1);
            return -1;
        }
        if (new_node->key < check_root_node->key)
            // move to left if the new_node value is smaller
            check_root_node = check_root_node->left;
        else
            // move to right if the new_node value is larger
            check_root_node = check_root_node->right;
    }
    if (new_node->key < tmp_node->key)
        tmp_node->left = new_node;
    else
        tmp_node->right = new_node;

    // unlock after inserting node
    pthread_mutex_unlock(&tmp_node->mutex);
    return 0;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
    // Remove node
    lab2_node* rm_node = tree->root;
    lab2_node* parent_rm_node = NULL;

    while(rm_node){
        // Find the object Node to remove
        if (rm_node->key == key)
            break;
        parent_rm_node = rm_node;

        if (rm_node->key > key)
            rm_node = rm_node->left;
        else
            rm_node = rm_node->right;
    }
    if (rm_node == NULL)
        // Return error if the node is Empty
        return -1;
    
    if (rm_node->left && rm_node->right){
        // If both L|R child exists
        lab2_node* save_node = rm_node->left;
        lab2_node* parent_save_node = rm_node;
        while (save_node->right){
            // get to the right end of the node
            parent_save_node = save_node;
            save_node = save_node->right;
        }
        rm_node->key = save_node->key;
        rm_node = save_node;
        parent_rm_node = parent_save_node;
    }
    
    lab2_node* tmp_node;
    if (rm_node->left == NULL)
        // set tmp_node if the node only has right node
        tmp_node = rm_node->right;
    if (rm_node == tree->root)
        tree->root = tmp_node;
    else{
        if (rm_node == parent_rm_node->left)
        // if current node is left child of parent_node
            parent_rm_node->left = tmp_node;
        else
        // if current node is right child of parent_node
            parent_rm_node->right = tmp_node;
    }

    // delete node
    lab2_node_delete(rm_node);
    return 1;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
    // Remove node with fine-grained lock
    lab2_node* rm_node = tree->root;
    lab2_node* parent_rm_node = rm_node;

    // lock before removing node
    pthread_mutex_lock(&thread_lock_1);
    while(1){
        // Find the object node to remove
        if (rm_node == NULL)
            break;
        if (rm_node->key == key){
            // if the node has the same key value to remove
            // apply lock
            pthread_mutex_lock(&thread_lock_2);
            break;
        }
        parent_rm_node = rm_node;

        if (key < rm_node->key)
            // move to left child if the key value is less than the node
            rm_node = rm_node->left;
        else
            // move to right child if the key value is larger than the node
            rm_node = rm_node->right;
    }
    pthread_mutex_unlock(&thread_lock_1);
    if (rm_node == NULL)
        // Return error if the node is Empty
        return -1;
    
    if (rm_node->left && rm_node->right){
        // If both L|R child exists
        lab2_node* save_node = rm_node->left;
        lab2_node* parent_save_node = rm_node;
        while (save_node->right){
            // get to the right end of the node
            parent_save_node = save_node;
            save_node = save_node->right;
        }
        rm_node->key = save_node->key;
        rm_node = save_node;
        parent_rm_node = parent_save_node;
    }
    
    lab2_node* tmp_node;
    if (rm_node->left == NULL)
        // set tmp_node if the node only has right node
        tmp_node = rm_node->right;
    if (rm_node == tree->root)
        tree->root = tmp_node;
    else{
        if (rm_node == parent_rm_node->left)
        // if current node is left child of parent_node
            parent_rm_node->left = tmp_node;
        else
        // if current node is right child of parent_node
            parent_rm_node->right = tmp_node;
    }

    // delete node
    lab2_node_delete(rm_node);
    pthread_mutex_unlock(&thread_lock_2);
    return 1;
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
    // Remove node coarse-grained lock

    // lock before removing node
    pthread_mutex_lock(&thread_lock_1);
    lab2_node* rm_node = tree->root;
    lab2_node* parent_rm_node = NULL;

    while(rm_node){
        // Find the object Node to remove
        if (rm_node == NULL)
            break;
        if (rm_node->key == key){
            // break if the same key value is found
            break;
        }
        parent_rm_node = rm_node;

        if (key < rm_node->key)
            // move to left if the key value is smaller than node
            rm_node = rm_node->left;
        else
            // move to right if the key value is larger than node
            rm_node = rm_node->right;
    }
    if (rm_node == NULL){
        // Return error if the node is Empty
        // after unlock
        pthread_mutex_unlock(&thread_lock_1);
        return -1;
    }
    
    if (rm_node->left && rm_node->right){
        // If both L|R child exists
        lab2_node* save_node = rm_node->left;
        lab2_node* parent_save_node = rm_node;
        while (save_node->right){
            // get to the right end of the node
            parent_save_node = save_node;
            save_node = save_node->right;
        }
        rm_node->key = save_node->key;
        rm_node = save_node;
        parent_rm_node = parent_save_node;
    }
    
    lab2_node* tmp_node;
    if (rm_node->left == NULL)
        // set tmp_node if the node only has right node
        tmp_node = rm_node->right;
    if (rm_node == tree->root)
        tree->root = tmp_node;
    else{
        if (rm_node == parent_rm_node->left)
        // if current node is left child of parent_node
            parent_rm_node->left = tmp_node;
        else
        // if current node is right child of parent_node
            parent_rm_node->right = tmp_node;
    }

    // delete node
    lab2_node_delete(rm_node);
    // unlock after removing the node
    pthread_mutex_unlock(&thread_lock_2);
    return 1;
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
    // Delete tree
    free(tree);
    tree = NULL;
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
    // Delete the node
    free(node);
    node = NULL;
}

