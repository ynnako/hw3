#include <stdlib.h>
#include <assert.h>

#include "defs.h"
#include "expTree.h"

//assitive functions declerations//
pElement recursive_find_element(pTree p_tree, pNode p_node, pKey p_key);
pElement recursive_eval(pTree p_tree, pNode p_node);
void TreeDestroyWrap(pTree p_tree, pNode p_node);
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

void TreeDestroy(pTree p_tree)

{		
	if (p_tree == NULL) return;
	TreeDestroyWrap( p_tree,p_tree->root);
	return;		
}

void TreeDestroyWrap(pTree p_tree,pNode p_node)
{   
	if (p_node==NULL) return;
	p_tree->delete_element(p_node->elem);
	pNode leftChild = p_node->Child;
	pNode rightChild = p_node->rightChild;
	free(p_node);
	if (p_node->rightChild==NULL && p_node->leftChild==NULL) return;
	TreeDestroyWrap( p_tree,rightChild);
	TreeDestroyWrap( p_tree,leftChild);
	return;
}


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

pElement TreeFindElement(pTree p_tree, pKey p_key)
{
	if (p_tree == NULL || p_key == NULL) return NULL;
	return recursive_find_element(p_tree, p_tree->root , p_key);
}

pElement TreeEvaluate(pTree p_tree)
{
	if (p_tree == NULL || p_tree->root == NULL) return NULL;
	return recursive_eval(p_tree, p_tree->root);
}


// asistive functions implementation
pElement recursive_eval(pTree p_tree,pNode p_node)
{
	if(p_node->leftChild==NULL || p_node->rightChild==NULL) return p_node->elem;
	pElement Result_left =recursive_eval(p_tree,p_node->leftChild);
	pElement Result_right =recursive_eval(p_tree,p_node->rightChild);
	return p_tree->operate_on_elements(p_node->elem,Result_left,Result_right);
}

pElement recursive_find_element(pTree p_tree, pNode p_node, pKey p_key)
{
	pElement p_left, p_right;
	if (p_node == NULL)return NULL;
	if (p_tree->CompareKeyFunction(p_key, p_tree->get_elements_key(p_node->elem))) return p_node->elem;/*check this condition*/
	p_left = recursive_find_element(p_tree, p_node->leftChild, p_key);
	p_right = recursive_find_element(p_tree, p_node->rightChild, p_key);
	if (p_left != NULL) return p_left;
	if (p_right != NULL) return p_right;
	return NULL;
}