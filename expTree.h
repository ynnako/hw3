#ifndef EXP_TREE_H
#define EXP_TREE_H

#include "defs.h"
/*hello,it's me*/
/* complete the definition of the pointers to Tree and Node */
typedef struct _node* pNode;
typedef struct _tree* pTree;

typedef void* pElement;
typedef void* pKey;

typedef pElement(*CloneFunction)(pElement e);
typedef void(*DelFunction)(pElement e);
typedef pElement(*OperateFunction)(pElement op,
									pElement left,
									pElement right);
typedef pKey(*GetKeyFunction)(pElement elem);
typedef Bool(*CompareKeyFunction)(const pKey key1, const pKey key2);

/* complete the definition of the interface functions */

pTree TreeCreate(CloneFunction,
					DelFunction,
					OperateFunction,
					GetKeyFunction,
					CompareKeyFunction);

void TreeDestroy(pTree);

pNode TreeAddRoot(pElement, pTree);

pNode TreeAddLeftChild(pTree, pNode, pElement);

pNode TreeAddRightChild(pTree, pNode, pElement);

pElement TreeFindElement(pTree, pKey);

pElement TreeEvaluate(pTree);

#endif	// EXP_TREE_H