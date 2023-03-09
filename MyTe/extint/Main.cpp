#include <windows.h>
#include "Main.h"
#include <tchar.h>
#include "optparse.h"

int _tmain(int argc, TCHAR* argv[]) {
	struct optparse_long longopts[] = {
		{TEXT("extint"), 'e', OPTPARSE_NONE},
		{TEXT("Ai"), 'a', OPTPARSE_REQUIRED},
		{TEXT("readpol"), 'p', OPTPARSE_REQUIRED},
		{TEXT("help"), '?', OPTPARSE_NONE},
		{TEXT("help"), 'h', OPTPARSE_NONE},
		{0}
	};
	int option;
	struct optparse options;
	wstring str;
	double a = 3.14;
	str = wformat(TEXT("%10.5f"), a);
	(void)argc;
	optparse_init(&options, argv);
	while ((option = optparse_long(&options, longopts, NULL)) != -1) {
		switch (option) {
		case 'e':
			return Extint(argc, argv);
		case 'a':
			return Ai(argc, argv);
		case 'p':
			return ReadPol(argc, argv);
		case 'h':
		case '?':
			fwprintf(stderr, TEXT("Usage: \n %s --extint --data .\\Data --mask *.* --out .\\ \n"), argv[0]);
			fwprintf(stderr, TEXT("%s\n"), options.errmsg);
			return(EXIT_FAILURE);
		default:
			fwprintf(stderr, TEXT("%s\n"), options.errmsg);
			return(EXIT_FAILURE);
		}
	}
	return 0;
}
