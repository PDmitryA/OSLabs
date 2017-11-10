//#include <unistd.h>
//#include <errno.h>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <fcntl.h>
#include <ctype.h>

unsigned char* recode(unsigned char* const buf);


int main() {
	int num;
	static unsigned char buf[BUFSIZ];
	while(fgets(buf, BUFSIZ, stdin) != NULL) {
		printf("%s", recode((unsigned char* const)buf));
	}
	return 0;
}

int isrusalpha(int c){
	if(c > 191 || c == 168 || c == 184)
		return 1;
	return 0;
}

unsigned char* recode(unsigned char* const buf) {
	unsigned char c;
	unsigned char variance;
	int i = 0;
	static unsigned char tab[] = {
		224-97, 225-98, 226-118, 227-103, 228-100, 229-101, 230-122, 231-122, 232-105, 233-121, 234-107, 235-108, 236-109, 237-110, 238-111, 239-112, 240-114, 241-99, 242-116, 243-117, 244-102, 245-104, 246-122, 247-99, 248-104, 249-104, 250-96, 251-105, 252-105, 253-101, 254-117, 255-97
	};
	for(; (c = buf[i]) != '\0'; i++) {
		if(isrusalpha((int)c)) {
			if((int)c == 168 || (int)c == 184){
				if((int)c == 184)
					variance = c-111;
				else
					variance = c-79;
			}
			else{
				variance = (unsigned char)tab[c-(c>223?224:192)];
			}
			buf[i] -= variance;
			//fprintf(stderr, "\nvariance:%d\nbuf[i]:%d\nbuf[i] - variance: %d\n", (int)variance, (int)buf[i]+variance, (int)buf[i]);
		}
	}
	return(buf);
}
