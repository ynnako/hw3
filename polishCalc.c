#include <stdlib.h>
#include <string.h>

#include "polishCalc.h"
#include "polishCalcTypes.h"
#include "expTree.h"


typedef CalcElement *PcalcElement;
static pTree p_tree = NULL;
static CalcElement static_element = {SYMBOL, NULL, ADD, 0};
static PcalcElement p_static_elem = &static_element;


/**********************************************************************************************
 *  function name:  create_element
 *  Description:    this function creates an element from the parsed string according to its
 *                  content
 *  Parameters:     @param elem_str - the parsed string.
 *  Return value:   the function returns a success value if the tree was built corectlly.
 ***********************************************************************************************/
Result create_element(char *elem_str) {
    if (elem_str == NULL || *elem_str == 0) return FAILURE;

    if (*elem_str == '+' || *elem_str == '*' || *elem_str == '/' || *elem_str == '-') {
        p_static_elem->type = OPERATOR;

        if (*elem_str == '+') p_static_elem->opType = ADD;

        else if (*elem_str == '*') p_static_elem->opType = MUL;

        else if (*elem_str == '/') p_static_elem->opType = DIV;

        else if (*elem_str == '-') p_static_elem->opType = SUB;

        p_static_elem->key = NULL;
    } else if (*elem_str >= '0' && *elem_str <= '9') {
        p_static_elem->type = OPERAND;

        p_static_elem->val = (float) atof(elem_str); //check with e (exponent)

        p_static_elem->key = NULL;
    } else {

        p_static_elem->type = SYMBOL;
        p_static_elem->val = 0;
        p_static_elem->key = elem_str;  //might need to free in clone element
    }

    return SUCCESS;
}

/**********************************************************************************************
 *  function name:  tree_build
 *  Description:    this function is a recursive function that builds the tree from  top to
 *                  by defining each node and nodes element according to the users command.
 *  Parameters:     @param p_tree - a pointer to the tree.
 *                  @param p_node - a pointer to a node.
 *                  @param str - a pointer to the stline
 *  Return value:   the function returns a success value if the tree was built correctly.
 ***********************************************************************************************/
Result tree_build(pTree p_tree, pNode p_node, char *str) {

    pNode p_new_node_left, p_new_node_right;
    if (p_tree == NULL || p_node == NULL || str == NULL || *str == 0)return FAILURE;
    //the conditions on str are for stopping the recursive call at end of line. the rest of the conditions are for standard input validation
    str = strtok(NULL, " "); //get next string for creating left element
    if (create_element(str) == FAILURE) return FAILURE; //try to create an element from the string. if failed - stop!

    p_new_node_left = TreeAddLeftChild(p_tree, p_node, p_static_elem);

    if (p_new_node_left == NULL) return FAILURE; // TreeAddLeftChild failed

    if (p_static_elem->type == OPERATOR) { // if an operator is added we need to add sons to the left recursively.
        if (tree_build(p_tree, p_new_node_left, str) == FAILURE)
            return FAILURE; // wasn't able to build tree due to bad input, memory problems
    }
    str = strtok(NULL, " "); //get next string for creating right element
    if (create_element(str) == FAILURE) return FAILURE; //try to create an element from the string. if failed - stop!

    p_new_node_right = TreeAddRightChild(p_tree, p_node, p_static_elem);

    if (p_new_node_right == NULL) return FAILURE;

    if (p_static_elem->type == OPERATOR) { // if an operator is added we need to add sons to the right recursively.
        if (tree_build(p_tree, p_new_node_right, str) == FAILURE) return FAILURE;
    }
    return SUCCESS;
}

/**********************************************************************************************
*  function name:   calc_element_cloner
*  Description:     this function gets an element and clones it
*  Parameters:      @param src_element - a pointer to the tree.
*  Return value:    a pointer to the new cloned element.
***********************************************************************************************/
pElement calc_element_cloner(pElement src_element) {
    if (src_element == NULL) return NULL;
    int key_len;
    PcalcElement p_src_element = (PcalcElement) src_element;
    PcalcElement p_new_element = (PcalcElement) malloc(sizeof(CalcElement));

    if (p_new_element == NULL) return NULL;

    p_new_element->type = p_src_element->type;

    if (p_src_element->key != NULL) { //this means there is a key to this element and we need to copy it.
        key_len = (strlen(p_src_element->key) + 1);
        p_new_element->key = (char *) malloc(sizeof(char) * key_len);
        if (p_new_element->key == NULL) {
            free(p_new_element);
            return NULL;
        }
        strcpy(p_new_element->key, p_src_element->key);
    } else p_new_element->key = NULL;

    p_new_element->opType = p_src_element->opType;

    p_new_element->val = p_src_element->val;

    return (PcalcElement) p_new_element;
}


/**********************************************************************************************
*  function name:   destroy_calc_element
*  Description:     this function get an element and frees its memory, including any memory allocated for its key
*  Parameters:      @param c_element - a pointer to the element.
*  Return value:    None.
***********************************************************************************************/
void destroy_calc_element(pElement c_element) {
    PcalcElement p_element = NULL;
    if (c_element == NULL) return;
    p_element = (PcalcElement) c_element; //cast in order to get access to its fields
    if (p_element->key != NULL) free(p_element->key); //free any allocated memory for the key.
    free(p_element);
}

/**********************************************************************************************
*  function name:   operate_function
*  Description:     this function gets three calc elements and calculates the operation of
*                   left on right.
*  Parameters:      @param op
*                   @param left
*                   @param right
*  Return value:    a pointer to an element that holds the result.
***********************************************************************************************/
pElement operate_function(pElement op, pElement left, pElement right) {
    if (op == NULL || left == NULL || right == NULL) return NULL;
    PcalcElement p_operator = (PcalcElement) op;
    PcalcElement p_operanL = (PcalcElement) left;
    PcalcElement p_operandR = (PcalcElement) right;
    PcalcElement p_result_elem = (PcalcElement) malloc(sizeof(CalcElement)); // this is the element that would hold the result
    float tmp_result = 0;

    if (p_result_elem == NULL) return NULL;
    switch (p_operator->opType) {

        case ADD  :
            tmp_result = p_operanL->val + p_operandR->val;
            break;

        case SUB  :
            tmp_result = p_operanL->val - p_operandR->val;
            break;

        case MUL  :
            tmp_result = p_operanL->val * p_operandR->val;
            break;

        case DIV  :
            if (p_operandR->val == 0) { //cant divide by zero
                free(p_result_elem); //this element was allocated as in the beginning of this function
                return NULL;
            }
            tmp_result = p_operanL->val / p_operandR->val;
            break;
    }
    p_result_elem->val = tmp_result;
    p_result_elem->key = NULL;
    return (PcalcElement) p_result_elem;
}

/**********************************************************************************************
*  function name:   get_calcElement_key
*  Description:     gets an element and returns a pointer to the value of key field
*  Parameters:      @param calc_element
*  Return value:    a pointer to the string containing the calc element's key
***********************************************************************************************/
pKey get_calcElement_key(pElement calc_element) {
    PcalcElement p_calc_element = (PcalcElement) calc_element;
    return p_calc_element->key;
}

/**********************************************************************************************
*  function name:   compare_keys
*  Description:     compares two keys
*  Parameters:      @param key1
*                   @param key2
*  Return value:    success = equal or failure = not equal
***********************************************************************************************/
Bool compare_keys(const pKey key1, const pKey key2) {
    if (key2 == NULL || key1 == NULL) return FALSE;
    if (strcmp((char *) key1, (char *) key2) == 0) return TRUE;
    return FALSE;
}


/**********************************************************************************************
*  function name:   InitExpression
*  Description:     builds a tree from the expression
*  Parameters:      @param exp - a pointer to a string that holds the expression.
*  Return value:    success = a tree was created or failure = failed to create a tree.
***********************************************************************************************/
Result InitExpression(char *exp) {
    char *new_exp = NULL;
    pNode p_root;
    if (p_tree != NULL) DeleteExpression();

    if (exp == NULL || *exp == 0) return FAILURE;

    p_tree = TreeCreate(calc_element_cloner, destroy_calc_element, operate_function, get_calcElement_key, compare_keys);

    if (p_tree == NULL) return FAILURE;

    new_exp = strtok(exp, " "); // get first operator/symbol/operand (for the root)

    if (create_element(new_exp) == FAILURE) return FAILURE;

    p_root = TreeAddRoot(p_static_elem, p_tree);

    if (p_root == NULL) return FAILURE;

    if (p_static_elem->type == OPERATOR) { // an operation needs to be preformed
        if (tree_build(p_tree, p_root, new_exp) == FAILURE) return FAILURE; // call recursive function and check result
    }
    new_exp = strtok(NULL, " "); //make sure we didnt get an invalid input by checking if we got to the end of the string
    if (new_exp != NULL) return FAILURE;
    return SUCCESS;
}

/**********************************************************************************************
*  function name:   SetSymbolVal
*  Description:     assigns a value to a given symbol
*  Parameters:      @param symName  - the name of the symbol.
*                   @param val - the value that would be assigned.
*  Return value:    success = the value was assigned or failure = no value assigned.
***********************************************************************************************/
Result SetSymbolVal(char *symName, float val) {
    if (symName == NULL) return FAILURE;/*fail*/
    PcalcElement p_2elem = TreeFindElement(p_tree, symName); //find the symbol inside the tree.
    if (p_2elem == NULL) return FAILURE;/*false*/
    p_2elem->val = val;
    return SUCCESS;

}
/**********************************************************************************************
*  function name:   EvaluateExpression
*  Description:     preform calculation of the calc tree's expression
*  Parameters:      @param res - a pointer which would hold the result at the end.
*  Return value:    success = calculation was successful or failure = unable to preform calculation.
***********************************************************************************************/
Result EvaluateExpression(float *res) {
    if (res == NULL) return FAILURE;
    PcalcElement p_2result = TreeEvaluate(p_tree);
    if (p_2result == NULL) return FAILURE;
    *res = p_2result->val;
    destroy_calc_element(p_2result);
    return SUCCESS;
}

/**********************************************************************************************
*  function name:   DeleteExpression
*  Description:     remove tree from memory
*  Parameters:      None
*  Return value:    None
***********************************************************************************************/
void DeleteExpression() {
    TreeDestroy(p_tree);
}

