#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Tick struct

typedef struct Tick {
	int referenceCount;
	time_t time;
	// Each tick object holds some state, here just 10MB of empty data
	char state[1024 * 1024 * 10]; 
} Tick;

Tick * newTick() {
	Tick * tick = calloc(1, sizeof(Tick));
	tick->referenceCount = 1;
	tick->time = time(NULL);
	return tick;
}

void freeTick(Tick * tick) {
	free(tick);
}

// Reference counting functions

Tick ** autoReleaseBuffer = NULL;
int autoReleaseBufferSize = 0;

Tick * makeAutoRelease(Tick * t) {
	if(autoReleaseBuffer == NULL) {
		autoReleaseBufferSize = 10;
		autoReleaseBuffer = calloc(autoReleaseBufferSize, sizeof(Tick *));
	}
	
	for(int i = 0; i < autoReleaseBufferSize; ++i) {
		if(autoReleaseBuffer[i] == NULL) {
			autoReleaseBuffer[i] = t;
			return t;
		}
	}
	
	// we couldn't find a free slot
	autoReleaseBuffer = realloc(autoReleaseBuffer, autoReleaseBufferSize * 2 * sizeof(Tick *));
	memset(autoReleaseBuffer + autoReleaseBufferSize, 0, autoReleaseBufferSize * sizeof(Tick *));
	autoReleaseBuffer[autoReleaseBufferSize] = t;	
	autoReleaseBufferSize *= 2;
	return t;
}

Tick * retain(Tick * t) {
	if(t != NULL) {
		t->referenceCount++;
	}
	return t;
}

void release(Tick * t) {
	if(t != NULL) {
		t->referenceCount--;
		if(t->referenceCount <= 0) {
			freeTick(t);
		}
	}
}

void drainAutoReleaseBuffer() {
	for(int i = 0; i < autoReleaseBufferSize; ++i) {
		if(autoReleaseBuffer[i] != NULL) {
			release(autoReleaseBuffer[i]);
			autoReleaseBuffer[i] = NULL;
		}
	}
}

// Tick handler functions

void logTick(Tick * tick) {
	printf("Tick: %s", ctime(&tick->time));
}

void deltaTick(Tick * tick) {
	static Tick * lastTick = NULL;
	if(lastTick == NULL) {
		lastTick = retain(tick);
		return;
	}
	
	int delta = (int)(tick->time - lastTick->time);
	release(lastTick);
	lastTick = retain(tick);
	
	if(delta > 5) {
		printf("%d seconds passed between ticks\n", delta);
	}
}

// main

int main(void) {
	char c;
	while((c = getchar())) {
		Tick * t = makeAutoRelease(newTick());
		logTick(t);
		deltaTick(t);
		drainAutoReleaseBuffer();

/* Removing the drainAutoReleaseBuffer would mean that 
every single time a tick gets created this tick will stay in memory
we would never delete it. also the array in which we store the 
pointers to our ticks would incread without end.
Therefore at some point the programm would crash since there is no 
more avaialble memory to allocate. 
*/


/* Refrence Counting is always usefull to make sure that all the objects for which we reserved some memmory are being release once we don't
need them anymore.
Reference Counting is especially usefull when we create s system where the amount of references to the objects that we use varie a lot.
A good example would be if we were to create an application that manages files.
if we represent each file as an object and there can be references in one file to the other. If we wanted to build a functionality that you can only delete a file if there are not more references from other files to that file. We would need to implement some sort of reference counting to make this possible.
*/
	}
}
