#include <stdlib.h>
#include <assert.h>

#include "defs.h"
#include "expTree.h"

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
	pTree p_new_tree;
	//pNode p_node;
	p_new_tree = (pTree)malloc(sizeof(Tree));
	//p_node = (pNode)malloc(sizeof(Node));
	if (p_new_tree == NULL/* || p_node == NULL*/) return NULL;
	p_node->elem = NULL;
	p_node->leftChild = NULL;
	p_node->rightChild = NULL;
	p_new_tree->root = p_node;
	p_new_tree->clone_element = clone_element;
	p_new_tree->delete_element = delete_element;
	p_new_tree->operate_on_elements = operate_on_elements;
	p_new_tree->get_elements_key = get_elements_key;
	p_new_tree->compare_keys = compare_keys;

	return p_new_tree;
}

void TreeDestroy(pTree p_tree)
{			
			return;		
}

pNode TreeAddRoot(pNode p_node, pTree p_tree)
{
	p_tree->root = p_tree->clone_element(p_node);//memory was allocated
	if (p_tree->root == NULL) return NULL;
	return p_tree->root;
}

pNode TreeAddLeftChild(pTree p_tree , pNode p_node , pElement p_elem)

