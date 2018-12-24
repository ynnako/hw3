#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "polishCalc.h"
#include "polishCalcTypes.h"
#include "defs.h"
#include "expTree.h"



typedef CalcElement* PcalcElement;
static pTree p_tree = NULL;
static CalcElement static_element = {0, NULL, 0, 0};
static PcalcElement p_static_elem = &static_element;

Result create_element(char* elem_str)
{

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
		p_static_elem->key = elem_str;  //might need to free in clone element
	}

	return SUCCESS;
}

Result tree_build(pTree p_tree, pNode p_node, char* str)
{
    str = strtok(NULL, " ");
    pNode p_new_node_left, p_new_node_right;
	if (p_tree == NULL || p_node == NULL || str == NULL || *str == 0)return FAILURE; //need to re think

	if (create_element(str) == FAILURE) return FAILURE;
	
	p_new_node_left = TreeAddLeftChild(p_tree, p_node, p_static_elem);
	
	if (p_new_node_left == NULL) return FAILURE;
	
	if (p_static_elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_new_node_left, str) == FAILURE) return FAILURE;
	}
    str = strtok(NULL, " ");
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
	int key_len;
    PcalcElement p_new_element=(PcalcElement)malloc(sizeof(CalcElement));
	if (p_new_element==NULL) return NULL;
		//e =(PcalcElement) e;/*casting*/
	PcalcElement p_element= (PcalcElement)	e;
		p_new_element->type = p_element->type;
		key_len = strlen(p_element->key) + 1;
		p_new_element->key = (char*)malloc(sizeof(char) * key_len);/*check if need to allocate*/
		if(p_new_element->key)
        {
		    free(p_new_element);
		    return NULL;
        }
		strcpy(p_new_element->key, p_element->key);
		p_new_element->opType = p_element->opType;
		p_new_element->val = p_element->val;
	return (PcalcElement) p_new_element;
}



void del_element(pElement e)
{
	/*check if need to free fields in the element*/
	if (e == NULL) return;
	PcalcElement p_elemt=(PcalcElement) e;
	if (p_elemt->key != NULL) free(p_elemt->key);
	free(p_elemt);
	return;
}

pElement operate_function(pElement op, pElement left, pElement right)
{
	if( op == NULL || left == NULL || right == NULL) return NULL;
    PcalcElement p_operator= (PcalcElement) op;
	PcalcElement p_operanL = (PcalcElement) left;
	PcalcElement p_operandR= (PcalcElement) right;
	float  tmp_result ;
    PcalcElement p_result_elem = (PcalcElement)malloc(sizeof(CalcElement));
    if(p_result_elem == NULL)
    {
        free(left);
        free(right);
        return NULL;
    }
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
    p_result_elem->val = tmp_result;
	//return result;
	free(left);
	free(right);
	return (PcalcElement) p_result_elem;
}
pKey get_key_p(pElement elem)
{
    PcalcElement p_elem = (PcalcElement) elem;
    if (p_elem->key == NULL) return NULL;
	
	return (char*) p_elem->key;
}

Bool compare_keys(const pKey key1, const pKey key2)
{
    if(key2 == NULL) return FALSE;
	if (strcmp((char*) key1,(char*) key2) == 0) return TRUE ;
	return FALSE;

}

Result InitExpression(char* exp)
{
	char* new_exp = NULL;
    pNode p_root;
	if (p_tree != NULL) DeleteExpression();

	if (exp == NULL || *exp == 0) return FAILURE;
	
	p_tree = TreeCreate(clone_function, del_element, operate_function, get_key_p, compare_keys);
	
	if (p_tree == NULL) return FAILURE;

    new_exp = strtok(exp, " ");

    if(create_element(new_exp) == FAILURE) return FAILURE;
	
	p_root = TreeAddRoot(p_static_elem , p_tree);
	
	if (p_root == NULL) return FAILURE;
	
	if (p_static_elem->type == OPERATOR)
	{
		if (tree_build(p_tree, p_root, new_exp) == FAILURE) return FAILURE;
	}
	
	return SUCCESS;
}

/* Set symbol value */
Result SetSymbolVal(char* symName, float val)
{
	if (symName==NULL) return FAILURE;/*fail*/
	PcalcElement p_2elem=TreeFindElement(p_tree,symName);
	if (p_2elem==NULL) return FAILURE;/*false*/
	p_2elem->val=val;
	return SUCCESS;

}

/* Evaluate expression */
Result EvaluateExpression(float *res)
{
	PcalcElement p_2result = TreeEvaluate(p_tree);
	if (p_2result == NULL) return FAILURE;
	 *res = p_2result->val;
	 free(p_2result);
	 return SUCCESS;
}

/* Destroy expression */
void DeleteExpression()
{
TreeDestroy(p_tree);
if(p_static_elem->key != NULL) free(p_static_elem->key);
}

