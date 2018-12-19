#include <stdlib.h>
#include <assert.h>

#include "polishCalc.h"
#include "polishCalcTypes.h"
#include "defs.h"
#include "expTree.h"


Result InitExpression(char* exp);

/* Set symbol value */
Result SetSymbolVal(char* symName, float val);

/* Evaluate expression */
Result EvaluateExpression(float *res);

/* Destroy expression */
void DeleteExpression();

pElement(*CloneFunction)(pElement e);

void(*DelFunction)(pElement e);

pElement(*OperateFunction)(pElement op,
	pElement left,
	pElement right);

pKey(*GetKeyFunction)(pElement elem);

Bool(*CompareKeyFunction)(const pKey key1, const pKey key2);