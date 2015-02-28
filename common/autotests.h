#ifndef AUTOTESTS__H
#define AUTOTESTS__H

#define TEST_OK		1
#define TEST_FAIL	0

// \brief File containing tests common for all 3 cards

/** \brief print table formatted test results
  * \param name -- test name
  * \param result -- result of the test, if it is not zero - prints OK, FAIL - otherwise
  */
#define TEST_RESULT(name,result)	{ \
	  puts("000|");\
	  puts(name);\
          puts("|");\
	  fflush(stdout);\
	  puts((result)?"OK":"FAIL");\
	  puts("\n\r");\
	}

/** \brief print table formatted test results (NN == no number)
  * \param name -- test name
  * \param result -- result of the test, if it is not zero - prints OK, FAIL - otherwise
  */
#define TEST_RESULT_NN(name,result)	{ \
	  puts("   |");\
	  puts(name);\
          puts("|");\
	  fflush(stdout);\
	  puts((result)?"OK":"FAIL");\
	  puts("\n\r");\
      }

int fpga_reg_autotest( int address );

#endif
