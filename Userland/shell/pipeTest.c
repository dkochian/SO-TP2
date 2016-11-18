//Crear tres binarios, spawner spawnea a s1 y s2,
//s1 y s2 comunican un dialogo hardcodeado, donde uno
//espera al otro para hablar

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void send(int fd);
void recieve(int fd);

void lock(int fd) {
	char toRead[1];
	while(read(fd, toRead, 1) < 1);
}

void step(int fd) {
	char * toWrite = "\n";
	while(write(fd, toWrite, 1) < 1);
}

int main(int argc, char * argv[]) {
	int s1tos2[2];
	int s2tos1[2];
	char * args[] = { NULL };
	int child1;
	int child2;

	if (pipe(s1tos2) != 0 || pipe(s2tos1) != 0) {
		printColor("Couldn't create pipe.", COLOR_ERROR);
		return 0;
	}

	if ((child1 = fork()) == 0) {
		printColor("failed to spawn sibling", COLOR_ERROR);
	} else if ((child2 = fork()) == 0) {
		printColor("failed to spawn sibling", COLOR_ERROR);
	}

	wPid(child1);
	wPid(child2);

	close(s1tos2[0]);
	close(s1tos2[1]);
	close(s2tos1[0]);
	close(s2tos1[1]);
}
int main(int argc, char * argv[]) {
	int myLock = 3;
	int myStep = 6;
	int otherLock = 5;
	int otherStep = 4;
	
	close(otherLock);
	close(otherStep);

	printf("Iniciando Lucia\n");
	step(myStep);
	lock(myLock);
	printf("L: ¿Quién es?\n");
	step(myStep);
	lock(myLock);
	printf("L: ¿Qué vienes a buscar?\n"); 
	step(myStep);
	lock(myLock);
	printf("L: Ya es tarde...\n");
	step(myStep);
	lock(myLock);
	printf("L: Porque ahora soy yo la que quiere estar sin ti...\n");
}
int main(int argc, char * argv[]) {
	int myLock = 5;
	int myStep = 4;
	int otherLock = 3;
	int otherStep = 6;

	close(otherLock);
	close(otherStep);

	lock(myLock);
	printf("J: Iniciando Joaquin\n");
	step(myStep);
	lock(myLock);
	printf("J: Soy yo...\n");
	step(myStep);
	lock(myLock);
	printf("J: A ti...\n"); 
	step(myStep);
	lock(myLock);
	printf("J: m¿Por qué?\n");
	step(myStep);
}

#include <stdio.h>
#include <unistd.h>

void send(int fd);
void recieve(int fd);

int main(int argc, char * argv[]) {
	int fds[2];
	int child;

	if (pipe(fds) != 0) {
		fprintf(stderr, "Couldn't create pipe.\n");
		return 0;
	}


	if ((child = fork()) == 0) {
		//Child
		close(fds[0]);
		send(fds[1]);
		close(fds[1]);
	} else if (child != -1) {
		//Parent
		close(fds[1]);
		recieve(fds[0]);
		close(fds[0]);
	} else {
		//Error
		fprintf(stderr, "Couldn't fork.\n");
	}

	return 0;
}

void send(int fd) {
	int c = 0;
	int w = 0;
	char writeBuffer[1024];
	int written = 0;
	
	while((c = getchar()) != EOF && w < 1024)
		writeBuffer[w++] = c;

	while(written < w)
		written += write(fd, writeBuffer + written, w - written);
}

void recieve(int fd) {
	int r = 0;
	int size = 1024;
	char readBuffer[1024];
	char * readPtr = readBuffer;

	while (size > 0 && (r = read(fd, readPtr, size))) {
		readPtr += r;
		size -= r;
	}

	readBuffer[readPtr - readBuffer] = '\0';

	printf("Read: %s\n", readBuffer);
}


