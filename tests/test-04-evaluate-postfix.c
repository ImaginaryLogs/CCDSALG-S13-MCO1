#include "../src/evaluate-postfix.h"
#include "test-utils.h"
#ifdef __linux__
    #include<unistd.h>
#endif

#ifndef __t_evaluate_h__
#define __t_evaluate_h__

#define LTPOST true
#define TEST_EVALUATIONS 2


int main(){
    signal(SIGSEGV, detectSegfault);
    struct testStatistics ts = createTestStatistics();
    String63 strFilenameInputActual = "t-evaluate-postfix-INPUT-ACTUAL.txt";
    String63 strFilenameInputExpect = "t-evaluate-postfix-OUTPUT-EXPECT.txt";
    String63 strInput       = "";
    String63 strCompared    = "";
    String63 strOutput      = "";
    FILE *fActual, *fExpect;
    int errEvalState = 0;
    int i = 0;
    

    OUT(LTPOST, "[EVAPST][START ] Testing Evaluate Postfix\n");
    
    fActual = fileValidOpen(strFilenameInputActual);
    fExpect = fileValidOpen(strFilenameInputExpect);

    for (i = 0; i < TEST_EVALUATIONS && !feof(fActual) && !feof(fExpect); i++){
        textline255Reader(fActual, strInput);
        textline255Reader(fExpect, strCompared);
        OUT(LTPOST, "[EVAPST][START ]: %s\n", strInput);

        //errEvalState = evaluatePostfix(strInput, strOutput);
        outputErrorCodes(errEvalState, strOutput);
        testCase(&ts, assertCaseString("Is the Postfix Output the Same as Expected Output?", strOutput, strCompared, true));
    }
    printTestStatistics(&ts);
    OUT(LTPOST, "\n[EVAPST][ENDING]: Testing Evaluate Postfix\n");

    fclose(fActual);
    fclose(fExpect);
    return 0;
}

#endif