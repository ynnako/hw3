#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "polishCalc.h"
#include "polishCalcTypes.h"
#include "defs.h"
#include "expTree.h"



static pTree p_tree = NULL;
static CalcElement static_element;
static pElement p_static_elem = &static_element;

Result tree_build(pTree p_tree, pNode p_node, char* str)
{
	if (p_tree == NULL || p_node == NULL || str == NULL || *str == 0)return FAILURE; //need to re think

	if (create_element(str) == FAILURE) return FAILURE;
	
	p_node->leftChild = p_tree->TreeAddLeftChild(p_tree, p_node, p_static_elem);
	
	if (p_node->leftChild == NULL) return FAILURE;
	
	if (p_node->leftChild->elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_node->leftChild, str) == FAILURE) return FAILURE;	
	}
	if (create_element(str) == FAILURE) return FAILURE;
	
	p_node->rightChild = p_tree->TreeAddRightChild(p_tree, p_node, new_element);
	
	if (p_node->rightChild == NULL) return FAILURE;
	
	if (p_node->rightChild->elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_node->rightChild, str) == FAILURE) return FAILURE;
	}	
	return SUCCESS;
}



Result create_element(char* elem_str)
{	
	elem_str = strtok(NULL, ' ');
	
	if (elem_str == NULL || *elem_str == 0) return FAILURE;

	if (*elem_str == '+' || *elem_str == '*' || *elem_str == '/' || *elem_str == '-')
	{
		p_static_elem->type = OPERATOR;
	
		if (*elem_str == '+') p_static_elem->opType = ADD;
		
		else if (*elem_str == '*') p_static_elem->opType = MUL;
		
		else if (*elem_str == '/') p_static_elem->opType = DIV;
		
		else if (*elem_str == '-') p_static_elem->opType = SUB;

		p_static_elem->key = NULL;
	}
	else if (*elem_str >= '0' && *elem_str <= '9')
	{
		p_static_elem->type = OPERAND;
		
		p_static_elem->val = atof(elem_str); //check with e (exponent)

		p_static_elem->key = NULL;
	}
	else 
	{
		p_static_elem->type = SYMBOL;
		p_static_elem->val = 0;
		p_static_elem->key = elem_str;  //check if i need to copy or is this ok
	}

	return;
}

CloneFunction name_clone(pElement e)
{

}

DelFunction neme_del(pElement e)
{

}

OperateFunction name_op(pElement op, pElement left, pElement right)
{

}

GetKeyFunction name_get(pElement elem)
{

}

CompareKeyFunction name_compare(const pKey key1, const pKey key2)
{

}

Result InitExpression(char* exp)
{
	if (p_tree != NULL) DeleteExpression();

	if (exp == NULL || *exp == 0) return FAILURE;
	
	p_tree = TreeCreate(name_clone, name_del, name_op, name_get, name_compare)
	
	if (p_tree == NULL)
	{
		free(p_tree);
	
		return FAILURE;
	}
	
	if(create_element(exp) == FAILURE) return FAILURE;
	
	p_tree->root = TreeAddRoot(p_static_elem , p_tree);
	
	if (p_tree->root == NULL)
	{
		DeleteExpression();
	
		return FAILURE;
	}
	
	if (p_tree->root->elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_tree->root, exp) == FAILURE)
		{
			DeleteExpression();
	
			return FAILURE;
		}
	}
	
	return SUCCESS;
}

/* Set symbol value */
Result SetSymbolVal(char* symName, float val)
{

}

/* Evaluate expression */
Result EvaluateExpression(float *res)
{

}

/* Destroy expression */
void DeleteExpression()
{

}

