
#ifndef _CONFIG_H_
#define _CONFIG_H_

#define EVALUATION_VERSION (1)

#pragma region LOGGING_CONSTS

#define DEBUG (true)
#define ENABLE_CLOCK (true)
#define ENABLE_LOG_OPERATIONS (true)
#define ENABLE_LOG_EVALUATE_POSTFIX (true)
#define ENABLE_LOG_STACK (true)
#define ENABLE_LOG_QUEUE (true)


#pragma endregion

#pragma region ANSI_COLOR_CODES

#define F_WHITE "\033[0;37m"
#define F_RED "\033[0;31m"
#define F_YELLOW "\033[0;33m"
#define F_GREEN "\033[0;32m"
#define F_BLUE "\033[0;34m"
#define F_CYAN "\033[0;36m"
#define F_BLACK "\033[0;30m"
#define F_NORMAL "\033[0m"

#define B_CYAN "\033[0;46m"
#define B_GREEN "\033[0;42m"
#define B_YELLOW "\033[0;43m"
#define B_RED "\033[0;41m"

#pragma endregion 

#endif