#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "polishCalc.h"
#include "polishCalcTypes.h"
#include "defs.h"
#include "expTree.h"



static pTree p_tree = NULL;

Result tree_build(pTree p_tree, pNode p_node, char* str)
{
	if (p_tree == NULL || p_node == NULL || str == NULL || *str == 0)return FAILURE; //need to re think

	CalcElement new_element = create_element(str);
	
	p_node->leftChild = p_tree->TreeAddLeftChild(p_tree, p_node, new_element);
	
	if (p_node->leftChild == NULL) return FAILURE;
	
	if (p_node->leftChild->elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_node->leftChild, str) == FAILURE) return FAILURE;	
	}
	new_element = create_element(str);
	
	p_node->rightChild = p_tree->TreeAddRightChild(p_tree, p_node, new_element);
	
	if (p_node->rightChild == NULL) return FAILURE;
	
	if (p_node->rightChild->elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_node->rightChild, str) == FAILURE) return FAILURE;
	}	
	return SUCCESS;
}



CalcElement create_element(char* elem_str)
{	
	CalcElement new_element;

	elem_str = strtok(NULL, ' ');
	
	if (*elem_str == '+' || *elem_str == '*' || *elem_str == '/' || *elem_str == '-')
	{
		new_element->type = OPERATOR;
		if (*elem_str == '+')
			new_element->opType = ADD;
		else if (*elem_str == '*')
			new_element->opType = MUL;
		else if (*elem_str == '/')
			new_element->opType = DIV;
		else if (*elem_str == '-')
			new_element->opType = SUB;
	}
	if (*elem_str >= '0' && *elem_str <= '9')
	{
		p_new_element->type = OPERAND;
		p_new_element->val = atof(elem_str); //check with e (exponent)

	}
	else if (*elem_str >= 'a' && *elem_str <= 'z' || *elem_str >= 'A' && *elem_str <='Z')
	{
		p_new_element->type = SYMBOL;
		p_new_element->val = 0;
		new_element->key = elem_str;  //check if i need to copy or is this ok
	}

	return new_element;
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
	p_tree->root = TreeAddRoot(create_element(exp) , p_tree);
	if (p_tree->root == NULL)
	{
		DeleteExpression();
		return FAILURE;
	}
	if (tree_build(p_tree, p_tree->root, exp) == FAILURE)
	{
		DeleteExpression();
		return FAILURE;
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

