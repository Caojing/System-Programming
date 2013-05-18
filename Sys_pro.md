###about define 1
	-#define PRINTD(x)	printf(#x " = %d\n", x)
	-#define PRINTS(x)	printf(#x " = %s\n", x)
	-#define PRINTP(x)	printf(#x " = %p\n", x)
	-#define else
	-#define PRINTD(x)
	-#define PRINTS(x)
	-#define PRINTP(x)

###about define 2
	-#define SIG_SET(x)	\
	-	void sig_set##x(int signo)	\
	-	{\
	-		msg = x;	\	
	-	}

	-SIG_SET(0);
	-SIG_SET(1);
	-SIG_SET(2);
