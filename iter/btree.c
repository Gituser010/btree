/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */


#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

stack_bst_t *stack;


/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  stack =malloc(sizeof(stack_bst_t));
  if(stack==NULL)
  {
    exit(-1);
  }
  (*tree)=NULL;
  return;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if(tree!=NULL)
  {
    while((tree->key!=key))
    {
    
      if(key<tree->key)
      {
        tree=tree->left;
      }
      else if(key>tree->key)
      {
        tree=tree->right;
      }
      if(tree==NULL){
        return false;
      }
    }
    if(tree!=NULL)
    {
      (*value)=tree->value;
      return true;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
 
  while ((*tree)!=NULL)
  {
    if(key>(*tree)->key)
    {
      tree=&((*tree)->right);
    }
    else if(key<(*tree)->key)
    {
      tree=&((*tree)->left);
    }
    else{
      (*tree)->value=value;
      break;
    }
  }

  if((*tree)==NULL)
  {
    (*tree)=malloc(sizeof(bst_node_t));
    if((*tree)==NULL)
    {
      exit(-1);
    }
    (*tree)->key=key;
    (*tree)->left=NULL;
    (*tree)->right=NULL;
    (*tree)->value=value;
    return;
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  while ((*tree)->right!=NULL)
  {
    tree=&((*tree)->right);
  }
  if((*tree)->left==NULL)
  {
    target->key=(*tree)->key;
    target->value=(*tree)->value;
    free(*tree);
    (*tree)=NULL;
  }
  else{
    bst_node_t *tmp;
    tmp=(*tree)->left;
    target->key=(*tree)->key;
    target->value=(*tree)->value;
    free(*tree);
    (*tree)=tmp;
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    while((*tree)->key!=key)  // najdenie polozky ktoru chceme vymazat
    {
      if(key<(*tree)->key){
        tree=&((*tree)->left);
      }
      else{
        tree=&((*tree)->right);
      }
      if((*tree)==NULL)
      {
        return;
      }
    }

    if((*tree)->left!=NULL && (*tree)->right!=NULL ){ // ak ma obidva podstromy
      bst_replace_by_rightmost((*tree),&((*tree)->left));
    }
    else // ak nema obidva podstromy
    {
      if((*tree)->left==NULL && (*tree)->right==NULL) //ak nema ani jeden
      {
        free(*tree);
        (*tree)=NULL;
      }
      else  // ak ma aspon jeden podstrom
      {
        if((*tree)->left!=NULL) // ak ma lavy podstrom
        {
          bst_node_t *tmp;
          tmp=(*tree)->left;
          free(*tree);
          (*tree)=tmp;
        }
        else //ak ma pravvy podstrom
        {
          bst_node_t *tmp;
          tmp=(*tree)->right;
          free(*tree);
          (*tree)=tmp; 
        }
      }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) { // pouzit stack;
  stack_bst_t *stack2;
  stack2=(stack_bst_t *)malloc(sizeof(stack_bst_t));
  if(stack2==NULL)
  {
    exit(-1);
  }

  stack_bst_init(stack);
  stack_bst_init(stack2);
  while ((*tree) != NULL)
  {
    stack_bst_push(stack,*tree);
    *tree = (*tree)->left;
  }
  while (!stack_bst_empty(stack))
  {
    *tree = stack_bst_top(stack);
    stack_bst_pop(stack);
    if((*tree)->right!=NULL)
    {
      (*tree)=(*tree)->right;
      while ((*tree)!=NULL)
      {
        stack_bst_push(stack,(*tree));
        stack_bst_push(stack2,(*tree));
        (*tree)=(*tree)->left;
      }
    }
  }
  while (!stack_bst_empty(stack2))
  {
    (*tree)=stack_bst_top(stack2);
    stack_bst_pop(stack2);
    free(*tree);
    (*tree)=NULL;
  }  
  return;
   
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL){
      stack_bst_push(to_visit,tree);
      bst_print_node(tree);
      tree = tree->left;
      }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_init(stack);
  bst_leftmost_preorder(tree,stack);
  while (!stack_bst_empty(stack))
  {
    tree=stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right,stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while (tree != NULL){
      stack_bst_push(to_visit,tree);
      tree = tree->left;
      }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_init(stack);
  bst_leftmost_inorder(tree,stack);
  while (!stack_bst_empty(stack))
  {
    tree=stack_bst_top(stack);
    stack_bst_pop(stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right,stack);
  } 
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
  stack_bool_t *stack_b;
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,stack_bool_t *first_visit) {
    while (tree!=NULL)
    {
      stack_bst_push(to_visit,tree);
      stack_bool_push(first_visit,true);
      tree=tree->left;
    }
    
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  bool fromLeft;
  stack_b=(stack_bool_t *)malloc(sizeof(stack_bool_t));
  if(stack_b==NULL)
  {
    exit(-1);
  }
  stack_bst_init(stack);
  stack_bool_init(stack_b);
  bst_leftmost_postorder(tree,stack,stack_b);
  while (!stack_bst_empty(stack))
  {
    tree= stack_bst_top(stack);
    fromLeft=stack_bool_top(stack_b);
    stack_bool_pop(stack_b);
    if(fromLeft)
    {
      stack_bool_push(stack_b,false);
      bst_leftmost_postorder(tree->right,stack,stack_b);
    }
    else
    {
      stack_bst_pop(stack);
      bst_print_node(tree);
    }
  }
  
}
