#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "polishCalc.h"
#include "polishCalcTypes.h"
#include "defs.h"
#include "expTree.h"



typedef CalcElement* PcalcElement;
static CalcElement result;
static PcalcElement p_result= &result;
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


CloneFunction clone_function(pElement e)
{
	PcalcElement p_new_element=(PcalcElement)malloc(sizeof(CalcElement));
	if (p_new_element==NULL) return NULL;
		e =(PcalcElement) e;/*casting*/
		p_new_element->type = e->type;
		p_new_element->key = e->key;/*check if need to allocate*/
		p_new_element->opType = e->opType;
		p_new_element->val = e->val;
	return p_new_element;
}



DelFunction del_element(pElement e)
{
	/*check if need to free fields in the element*/
	PcalcElement p_elemt=(PcalcElement) e;
	free(p_elemt);
}

OperateFunction operate_function(pElement op, pElement left, pElement right)
{
	PcalcElement p_operator= (PcalcElement) op;
	PcalcElement p_operanL = (PcalcElement) left;
	PcalcElement p_operandR= (PcalcElement) right;
	float result ;
 
	switch(p_operator->opType) {

		case ADD  :
			result= p_operanL->val+p_operandR->val;
		break; 
	
		case SUB  :	
			result= p_operanL->val-p_operandR->val;
		break; 
		
		case MUL  :
			result= p_operanL->val*p_operandR->val;
		break; 
	
		case DIV  :
			result= p_operanL->val/p_operandR->val;
		break; 
	}
	p_result->val=result;
	//return result;  
	 return p_result; 
}

GetKeyFunction get_key_p(pElement elem)
{
	PcalcElement p_elem = (PcalcElement) elem;
	if (p_elem->key == NULL) return NULL;
	
	return p_elem->key;
}

CompareKeyFunction compare_keys(const pKey key1, const pKey key2)
{
	char* key1= (char*)key1;
	char* key2= (char*)key2;
  return strcmp(key1,key2);

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
	if (symName==NULL) return 0;/*fail*/
	PcalcElement p_2elem=TreeFindElement(p_tree,symName);
	if (p_2elem==NULL) return 0;/*false*/
	p_2elem->val=val;
	return 1;

}

/* Evaluate expression */
Result EvaluateExpression(float *res)
{
PcalcElement p_2result=TreeEvaluate(p_tree);
if (p_2result==NULL) return 0;
 *res->p_2result->val;
 return 1;
}

/* Destroy expression */
void DeleteExpression()
{
TreeDestroy(p_tree);
}

