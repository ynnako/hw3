#include <stdlib.h>
#include <assert.h>

#include "polishCalc.h"
#include "polishCalcTypes.h"
#include "defs.h"
#include "expTree.h"

typedef CalcElement* PcalcElement;
static CalcElement result;
PcalcElement p_result= &result;

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

