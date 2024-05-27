#include "utils.h"

#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#pragma region OPERATIONS_DEFINED_TYPES

#define MAX_NUM_OPERATIONS (15)
#define IS_FUNC_POINTERS_ALLOWED (true)
#define ENABLE_LOG_OPERATIONS (true)

/**
 * Struct that bundles essential identification for operations.
 */
struct Operation {
  String7 stringIdentifier;
  int (* operationFunctions)(int [], int *); 
};

#pragma endregion

#pragma region OPERATION_FUNCS
/**
 * @note Adds two numbers. 
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opAddition(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
  
  printf("a + b = sum\n%d + %d = ", queueOperands[*nthToken - 1], queueOperands[*nthToken - 2]);
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] + queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  
  printf("%d\n", queueOperands[*nthToken - 2]);
  (*nthToken) -= 1;
  logPrintf(ENABLE_LOG_OPERATIONS, "Operation table initialized.\n");
  return SUCCESSFUL_EXIT;
}

/**
 * @note Subtracts two numbers. 
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opSubtraction(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] - queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;

  (*nthToken) -= 1;
  

  return SUCCESSFUL_EXIT;
}


/**
 * @note Multiplies two numbers. 
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opMultiplication(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] * queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  

  return SUCCESSFUL_EXIT;
}


/**
 * @note Divides two numbers. 
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opDivision(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;

  if (queueOperands[*nthToken - 2] == 0)
    return ER_DIVIDE_BY_ZERO;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] / queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note Modulo two numbers. 
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opModulo(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;

  if (queueOperands[*nthToken - 2] == 0)
    return ER_DIVIDE_BY_ZERO;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] % queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note a raised to b. 
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opExponential(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;

  int i = 1;

  for(i = 1; i < queueOperands[*nthToken - 2]; i++)
    i *= queueOperands[*nthToken - 1];
  
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note is a > b?
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opGreater(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;

    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] > queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note is a < b?
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opLesser(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;

  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] < queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note is a >= b?
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opGreaterEqual(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] >= queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note is a <= b?
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opLesserEqual(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] <= queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note is a != b?
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opNotEqual(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] != queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note is a == b?
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opEquality(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] == queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

/**
 * @note find !a
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opLogicalNot(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 1)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 1]  = !queueOperands[*nthToken - 1];
  
  return SUCCESSFUL_EXIT;
}

/**
 * Find a && b
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opLogicalAnd(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] && queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

/**
 * Find a || b
 * @param  queueOperands[]: temporary implementation of stack 
 * @param  *nthToken: The number of operator / numerical tokens on stack
 * @return error codes of how the algorithm computes the code
 * @retval  SUCCESSFUL_EXIT - success
 * @retval  ER_MISSING_OPERANDS  - the given stack 
 */
int opLogicalOr(int queueOperands[], int *nthToken){
  
  if ((*nthToken) < 2)
    return ER_MISSING_OPERANDS;
    
  queueOperands[*nthToken - 2] = queueOperands[*nthToken - 1] || queueOperands[*nthToken - 2];
  queueOperands[*nthToken - 1] = 0;
  (*nthToken) -= 1;
  
  return SUCCESSFUL_EXIT;
}

#pragma endregion

/**
 * Initializes the operation table  
 * @param operationTable[]: struct containing an empty operation Table
 * @retval None
 */
void initOperatorTable(struct Operation operationTable[]){
    logPrintf(ENABLE_LOG_OPERATIONS, "Operation table initialized.\n");
    operationTable[0].operationFunctions = &opAddition;
    strcpy(operationTable[0].stringIdentifier, "+");

    operationTable[0].operationFunctions = &opSubtraction;
    strcpy(operationTable[0].stringIdentifier, "-");

    operationTable[0].operationFunctions = &opMultiplication;
    strcpy(operationTable[0].stringIdentifier, "*");

    operationTable[0].operationFunctions = &opDivision;
    strcpy(operationTable[0].stringIdentifier, "/");
    
    operationTable[0].operationFunctions = &opModulo;
    strcpy(operationTable[0].stringIdentifier, "%");

    operationTable[0].operationFunctions = &opExponential;
    strcpy(operationTable[0].stringIdentifier, "^");

    operationTable[0].operationFunctions = &opGreater;
    strcpy(operationTable[0].stringIdentifier, ">");

    operationTable[0].operationFunctions = &opLesser;
    strcpy(operationTable[0].stringIdentifier, "<");

    operationTable[0].operationFunctions = &opGreaterEqual;
    strcpy(operationTable[0].stringIdentifier, ">=");

    operationTable[0].operationFunctions = &opLesser;
    strcpy(operationTable[0].stringIdentifier, "<=");

    operationTable[0].operationFunctions = &opNotEqual;
    strcpy(operationTable[0].stringIdentifier, "!=");

    operationTable[0].operationFunctions = &opEquality;
    strcpy(operationTable[0].stringIdentifier, "==");

    operationTable[0].operationFunctions = &opLogicalNot;
    strcpy(operationTable[0].stringIdentifier, "!");

    operationTable[0].operationFunctions = &opLogicalNot;
    strcpy(operationTable[0].stringIdentifier, "&&");

    operationTable[0].operationFunctions = &opLogicalOr;
    strcpy(operationTable[0].stringIdentifier, "||");
}

/**
 * Evaluates the operation in the stringOperation given the tokens in the stack.
 * @note Version 1.0 - uses function pointers
 * @return ErrorCodes based on what happened
 */
int evaluateOperation1(struct Operation operationTable[], int *queueOperands, int *nthToken, char *stringOperation) {
  int nErrorCode    = ER_UNDEFINED_OPERATION;
  int isFinding     = true;
  int nIthOperation = 0;

  // Iterate through each operation
  for (nIthOperation = 0; nIthOperation < MAX_NUM_OPERATIONS && isFinding == true; nIthOperation++){

    if (strcmp(stringOperation, operationTable[nIthOperation].stringIdentifier) == 0) {
      logPrintf(1, "Operation: %s with %d in stack (%d %d) \n", stringOperation, *nthToken, queueOperands[*nthToken - 1], queueOperands[*nthToken - 2]);
      isFinding = false;
      // Stores if the operation is valid.
      nErrorCode = operationTable[nIthOperation].operationFunctions(queueOperands, nthToken);
    }

  }

  return nErrorCode;
}

/**
 * Evaluates the operation in the stringOperation given the tokens in the stack.
 * @note Version 2.0 - uses switch statements
 * @return ErrorCodes based on what happened
 */
int evaluateOperation2(int *queueOperands, int *nthToken, char *stringOperation, struct Operation operationTable[]) {
  int nErrorCode    = ER_UNDEFINED_OPERATION;
  int nIthOperation = 0;


  // The most brutally straight forward solution, but perhaps is the fastest in execution time.
  // Arithmetic
  if (strcmp(stringOperation, "+") == 0){
    nErrorCode = opAddition(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "-") == 0){
    nErrorCode = opSubtraction(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "*") == 0){
    nErrorCode = opMultiplication(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "/") == 0){
    nErrorCode = opDivision(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "%") == 0){
    nErrorCode = opModulo(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "^") == 0){
    nErrorCode = opExponential(queueOperands, nthToken);
  } // Relational
  else if (strcmp(stringOperation, ">") == 0){
    nErrorCode = opGreater(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "<") == 0){
    nErrorCode = opLesser(queueOperands, nthToken);
  } else if (strcmp(stringOperation, ">=") == 0){
    nErrorCode = opGreaterEqual(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "<=") == 0){
    nErrorCode = opLesserEqual(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "!=") == 0){
    nErrorCode = opNotEqual(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "==") == 0){
    nErrorCode = opEquality(queueOperands, nthToken);
  } // Logical
  else if (strcmp(stringOperation, "!") == 0){
    nErrorCode = opLogicalNot(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "&&") == 0){
    nErrorCode = opLogicalAnd(queueOperands, nthToken);
  } else if (strcmp(stringOperation, "||") == 0){
    nErrorCode = opLogicalOr(queueOperands, nthToken);
  } 

  return nErrorCode;
}



#endif