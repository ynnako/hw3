#include <stdlib.h>
#include <assert.h>

#include "defs.h"
#include "expTree.h"


pElement recursive_find_element(pTree p_tree, pNode p_node, pKey p_key);
pElement recursive_eval(pTree p_tree, pNode p_node);
void recursive_tree_destroy(pTree p_tree, pNode p_node);
//-------------------------------//


/* definition of a node in the tree */
typedef struct _node
{
	pElement	elem;

	pNode		leftChild;
	pNode		rightChild;
} Node;

/* definition of the tree structure */   
typedef struct _tree
{
	pNode root;

	CloneFunction		clone_element;
	DelFunction			delete_element;
	OperateFunction		operate_on_elements;
	GetKeyFunction		get_elements_key;
	CompareKeyFunction	compare_keys;
	/* *** complete the definition of the tree structure *** */
} Tree;

/**********************************************************************************************
*  function name:   TreeCreate
*  Description:     instantiates a new tree struct
*  Parameters:      @param clone_element - a pointer to a function
*                   @param delete_element - a pointer to a function
*                   @param operate_on_elements  - a pointer to a function
*                   @param get_elements_key - a pointer to a function
*                   @param compare_keys - a pointer to a function
*  Return value:    a pointer to the new tree
***********************************************************************************************/
pTree TreeCreate(CloneFunction clone_element, DelFunction delete_element, OperateFunction operate_on_elements, GetKeyFunction get_elements_key, CompareKeyFunction compare_keys)
{
	if (clone_element == NULL || delete_element == NULL || operate_on_elements == NULL || get_elements_key == NULL || compare_keys == NULL) return NULL; 
	pTree p_new_tree;

	p_new_tree = (pTree)malloc(sizeof(Tree));
	if (p_new_tree == NULL) return NULL;
	p_new_tree->root = NULL;
	p_new_tree->clone_element = clone_element;
	p_new_tree->delete_element = delete_element;
	p_new_tree->operate_on_elements = operate_on_elements;
	p_new_tree->get_elements_key = get_elements_key;
	p_new_tree->compare_keys = compare_keys;

	return p_new_tree;
}


/**********************************************************************************************
*  function name:   TreeDestroy
*  Description:     free's all the memory that was allocated when creating it
*  Parameters:      @param p_tree  - a pointer to the tree.
*  Return value:    None.
***********************************************************************************************/
void TreeDestroy(pTree p_tree)
{		
	if (p_tree == NULL) return;
    recursive_tree_destroy(p_tree, p_tree->root);
	free(p_tree);
}

/**********************************************************************************************
*  function name:   recursive_tree_destroy
*  Description:     free's all the in tree's node recusively
*  Parameters:      @param p_tree - a pointer to the tree.
*                   @param p_node - a pointer to a specific node
*  Return value:    None.
***********************************************************************************************/
void recursive_tree_destroy(pTree p_tree, pNode p_node)
{
    if (p_node==NULL) return;
    recursive_tree_destroy(p_tree, p_node->leftChild);
    recursive_tree_destroy(p_tree, p_node->rightChild);
    if(p_node->elem != NULL) p_tree->delete_element(p_node->elem);
    free(p_node);
}

/**********************************************************************************************
*  function name:   TreeAddRoot
*  Description:     gets an element clones it and assigns it to the root's element field
*  Parameters:      @param p_elem - element to be cloned
*                   @param p_tree - the pointer to the treer
*  Return value:    a pointer to a node. in fact a pointer to rhe root of the tree
***********************************************************************************************/
pNode TreeAddRoot(pElement p_elem, pTree p_tree)
{
	if (p_tree == NULL || p_elem == NULL ) return NULL;
	pNode p_new_node;
	p_new_node = (pNode)malloc(sizeof(Node));
	if (p_new_node == NULL) return NULL;
	p_tree->root = p_new_node;
	p_new_node->elem = p_tree->clone_element(p_elem);//memory was allocated
	if (p_new_node->elem == NULL)
	{
		free(p_new_node);
		return NULL;
	}
	p_new_node->leftChild = NULL;
	p_new_node->rightChild = NULL;
	return p_tree->root;
}

/**********************************************************************************************
*  function name:   TreeAddLeftChild
*  Description:     allocates memory for a new node and assigns the pointer to the leftchild field of the node
*  Parameters:      @param p_tree - a pointer to the tree which the node is in
*                   @param p_node - a pointer to the "father" node
*                   @param p_elem - a pointer to the element that should be assigned to the new node.
*  Return value:    a pointer to a node. in fact a pointer to rhe left child
***********************************************************************************************/
pNode TreeAddLeftChild(pTree p_tree, pNode p_node, pElement p_elem)
{
	if (p_tree == NULL || p_node == NULL || p_elem == NULL) return NULL;
	pNode p_new_node;
	p_new_node = (pNode)malloc(sizeof(Node));
	if (p_new_node == NULL)return NULL;
	p_new_node->elem = p_tree->clone_element(p_elem);//memory was allocated
	if (p_new_node->elem == NULL)
	{
		free(p_new_node);
		return NULL;
	}
	p_new_node->leftChild = NULL;
	p_new_node->rightChild = NULL;
	p_node->leftChild = p_new_node;
	return p_new_node;
}

/**********************************************************************************************
*  function name:   TreeAddLeftChild
*  Description:     allocates memory for a new node and assigns the pointer to the rightchild field of the node
*  Parameters:      @param p_tree - a pointer to the tree which the node is in
*                   @param p_node - a pointer to the "father" node
*                   @param p_elem - a pointer to the element that should be assigned to the new node.
*  Return value:    a pointer to a node. in fact a pointer to rhe right child
***********************************************************************************************/
pNode TreeAddRightChild(pTree p_tree, pNode p_node, pElement p_elem)
{
	if (p_tree == NULL || p_node == NULL || p_elem == NULL) return NULL;
	pNode p_new_node;
	p_new_node = (pNode)malloc(sizeof(Node));
	if (p_new_node == NULL)return NULL;
	p_new_node->elem = p_tree->clone_element(p_elem);//memory was allocated
	if (p_new_node->elem == NULL)
	{
		free(p_new_node);
		return NULL;
	}
	p_new_node->leftChild = NULL;
	p_new_node->rightChild = NULL;
	p_node->rightChild = p_new_node;
	return p_new_node;
}

/**********************************************************************************************
*  function name:   TreeFindElement
*  Description:     searces down at tree for a given key
*  Parameters:      @param p_tree - the pointer to the tree which would be searched
*                   @param p_key
*  Return value:    a pointer to a node. in fact a pointer to rhe left child
***********************************************************************************************/
pElement TreeFindElement(pTree p_tree, pKey p_key)
{
	if (p_tree == NULL || p_key == NULL) return NULL;
	return recursive_find_element(p_tree, p_tree->root , p_key);
}

/**********************************************************************************************
*  function name:   TreeEvaluate
*  Description:     preforms an evaluation on a tree
*  Parameters:      @param p_tree - the pointer to the tree which would be evaluated
*  Return value:    a pointer to a node the holds the result of the evaluation.
***********************************************************************************************/
pElement TreeEvaluate(pTree p_tree)
{
	if (p_tree == NULL || p_tree->root == NULL) return NULL;
	return recursive_eval(p_tree, p_tree->root);
}

/**********************************************************************************************
*  function name:   recursive_eval
*  Description:     an insider function of TreeEvaluate. the function evaluates the tree recursively
*  Parameters:      @param p_tree - a pointer to the tree being evaluated
*                   @param p_node - a pointer to the node being evaluated
*  Return value:    a pointer to a node the holds the result of the evaluation.
***********************************************************************************************/
pElement recursive_eval(pTree p_tree,pNode p_node) {
    pElement Result_left = NULL, Result_right = NULL;
    if (p_node->leftChild == NULL || p_node->rightChild == NULL) { // we know that the tree would have only nodes with tow sons
        pElement p_operand = p_tree->clone_element(p_node->elem);
        return p_operand; //once we got to a leaf we should return a copy of the node's element
    }
    Result_left = recursive_eval(p_tree, p_node->leftChild);
    if (Result_left == NULL) return NULL;
    Result_right = recursive_eval(p_tree, p_node->rightChild);
    if (Result_right == NULL)
    {
        p_tree->delete_element(Result_left);
        return NULL;
    }
    pElement p_total_result = p_tree->operate_on_elements(p_node->elem,Result_left,Result_right);
    p_tree->delete_element(Result_left);
    p_tree->delete_element(Result_right);
    return p_total_result;
}

/**********************************************************************************************
*  function name:   recursive_find_element
*  Description:     an insider function of TreeFindElement. the function finds the key by
*                   searching down the tree  recursively
*  Parameters:      @param p_tree - a pointer to the tree
*                   @param p_node - a pointer to the current node
*  Return value:    a pointer to a node that holds the wanted key.
***********************************************************************************************/
pElement recursive_find_element(pTree p_tree, pNode p_node, pKey p_key)
{
	pElement p_left, p_right;
	if (p_node == NULL || p_key == NULL || p_tree == NULL) return NULL;
	if (p_tree->compare_keys(p_key, p_tree->get_elements_key(p_node->elem)) != 0) return p_node->elem;/*check this condition*/
	p_left = recursive_find_element(p_tree, p_node->leftChild, p_key); //look in the left side of the node
	p_right = recursive_find_element(p_tree, p_node->rightChild, p_key); //look in the right side of the node
	if (p_left != NULL) return p_left;
	if (p_right != NULL) return p_right;
	return NULL;
}