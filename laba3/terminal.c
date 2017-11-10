#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

int textmode(int);
int randint(int);
int getch();

int textmode(int mode) {
	static struct termios con[2];
	if(mode > 0)
		return(tcsetattr(0, TCSAFLUSH, &con[1]));
	tcgetattr(0, &con[0]);
	tcgetattr(0, &con[1]);
	con[0].c_lflag &= ~(ICANON | ECHO | ISIG);
	con[0].c_iflag &= ~(ISTRIP | IXOFF | IXANY | IXON);
	con[0].c_oflag |= CS8;
	con[0].c_cc[VMIN] = 2;
	con[0].c_cc[VTIME] = 1;
	return(tcsetattr(0, TCSAFLUSH, &con[0]));
}

int getch() {
	unsigned char c[2];
	c[0] = c[1] = 0;
	if(read(0, c, 2) < 2)
  		return(c[0]);
	if(c[0] == 27)
  		c[0] = 0;
	ungetc(c[1], stdin);
	return(c[0]);
}

int makestr(long long patternInt, char* pattern){

	
	sprintf(pattern, "%lld", patternInt);
	return strlen(pattern);

	/*
	int len = 0;
	//int cipher;
	while(patternInt > 0){
		pattern[len++] = (char) ((patternInt % 10) + 48);
		patternInt /= 10;
	}
	pattern[len]='\0';
	return len;*/
}

int getNumber(){
	int newNumber = 0;
	int cipher = 0;
	int became = 0;
	int minus = 0;
	while((int)read(0, &cipher, 1))
	if(isdigit(cipher)){
		newNumber = newNumber * 10 + cipher - 48;
		became = 1;
	}
	else {
		ungetc(cipher, stdin);
		fflush(stdin);
		return minus?-newNumber:newNumber;
	}
}

int main() {
	char pattern[80];
	long long patternInt;
	int len;
	int i = 0;
	int err = 0;
	unsigned char c;


	long long getnumb;
	getnumb = (long long)getNumber();

	srand(getpid());
	patternInt = (long long)rand();
	//fprintf(stderr, "%lld\n%s\n", patternInt, pattern);
	len = makestr(patternInt, pattern);
	write(1, pattern, len);
	write(1, "\n", 1);


	//getnumb = (long long)getNumber();
	//fprintf(stderr, "GETNUMBER: %lld\n", getnumb);
	//patternInt *= getnumb;

	patternInt *= getnumb;
	makestr(patternInt, pattern);
	
	//fprintf(stderr, "%lld\n%s\n", patternInt, pattern);
	
	textmode(0);
	while(i < len) {
	  switch(c = getch()) {
	    case 0:  c = '\007';
	             switch(getch()) {
	               case 67: c = pattern[i];
	                        break;
	               case 68: if(i == 0)
	                          break;
	                        i--;
	                        write(1, "\b", 1);
	                        continue;
	               default: break;
	              }
	             break;
	    case 27: i = len;
	             c = '\007';
	             break;
	    default: if(c != pattern[i])
	               c = '\007';
	             break;
	  }
	  (c == '\007') ? err++ : i++;
	  write(1, &c, 1);
	}
	write(1, "\n", 1);
	textmode(1);
	return(err);
}

