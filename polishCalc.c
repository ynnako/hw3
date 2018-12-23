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
static PcalcElement p_static_elem = &static_element;

Result create_element(char* elem_str)
{
	elem_str = strtok(NULL, " ");

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

	return SUCCESS;
}

Result tree_build(pTree p_tree, pNode p_node, char* str)
{
	pNode p_new_node_left, p_new_node_right;
	if (p_tree == NULL || p_node == NULL || str == NULL || *str == 0)return FAILURE; //need to re think

	if (create_element(str) == FAILURE) return FAILURE;
	
	p_new_node_left = TreeAddLeftChild(p_tree, p_node, p_static_elem);
	
	if (p_new_node_left == NULL) return FAILURE;
	
	if (p_static_elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_new_node_left, str) == FAILURE) return FAILURE;
	}
	if (create_element(str) == FAILURE) return FAILURE;
	
	p_new_node_right = TreeAddRightChild(p_tree, p_node, p_static_elem);
	
	if (p_new_node_right == NULL) return FAILURE;
	
	if (p_static_elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_new_node_right, str) == FAILURE) return FAILURE;
	}	
	return SUCCESS;
}



pElement clone_function(pElement e)
{
	PcalcElement p_new_element=(PcalcElement)malloc(sizeof(CalcElement));
	if (p_new_element==NULL) return NULL;
		//e =(PcalcElement) e;/*casting*/
	PcalcElement p_element= (PcalcElement)	e;
		p_new_element->type = p_element->type;
		p_new_element->key = p_element->key;/*check if need to allocate*/
		p_new_element->opType = p_element->opType;
		p_new_element->val = p_element->val;
	return (PcalcElement) p_new_element;
}



void del_element(pElement e)
{
	/*check if need to free fields in the element*/
	PcalcElement p_elemt=(PcalcElement) e;
	free(p_elemt);
	return;
}

pElement operate_function(pElement op, pElement left, pElement right)
{
	PcalcElement p_operator= (PcalcElement) op;
	PcalcElement p_operanL = (PcalcElement) left;
	PcalcElement p_operandR= (PcalcElement) right;
	float  tmp_result ;
 
	switch(p_operator->opType) {

		case ADD  :
			tmp_result = p_operanL->val+p_operandR->val;
		break; 
	
		case SUB  :
			tmp_result = p_operanL->val-p_operandR->val;
		break; 
		
		case MUL  :
			tmp_result = p_operanL->val*p_operandR->val;
		break; 
	
		case DIV  :
			tmp_result = p_operanL->val/p_operandR->val;
		break; 
	}
	p_result->val = tmp_result;
	//return result;  
	 return (PcalcElement) p_result;
}

pKey get_key_p(pElement elem)
{
	PcalcElement p_elem = (PcalcElement) elem;
	if (p_elem->key == NULL) return NULL;
	
	return (char*) p_elem->key;
}

Bool compare_keys(const pKey key1, const pKey key2)
{

	if (strcmp((char*) key1,(char*) key2) == 0) return TRUE ;
	return FALSE;

}

Result InitExpression(char* exp)
{
	pNode p_root;
	if (p_tree != NULL) DeleteExpression();

	if (exp == NULL || *exp == 0) return FAILURE;
	
	p_tree = TreeCreate(clone_function, del_element, operate_function, get_key_p, compare_keys);
	
	if (p_tree == NULL)
	{
		free(p_tree);
	
		return FAILURE;
	}
	
	if(create_element(exp) == FAILURE) return FAILURE;
	
	p_root = TreeAddRoot(p_static_elem , p_tree);
	
	if (p_root == NULL)
	{
		DeleteExpression();
	
		return FAILURE;
	}
	
	if (p_static_elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_root, exp) == FAILURE)
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
	PcalcElement p_2result = TreeEvaluate(p_tree);
	if (p_2result == NULL) return FAILURE;
	 *res = p_2result->val;
	 return SUCCESS;
}

/* Destroy expression */
void DeleteExpression()
{
TreeDestroy(p_tree);
}

