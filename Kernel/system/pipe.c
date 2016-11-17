#include "include/pipe.h"

#include "include/mmu.h"
#include "scheduler/include/scheduler.h"

#define DEBUG_PIPES 0
/*
static inline size_t pipe_unread(pipe_device_t * pipe) {
	if (pipe->read_ptr == pipe->write_ptr) {
		return 0;
	}
	if (pipe->read_ptr > pipe->write_ptr) {
		return (pipe->size - pipe->read_ptr) + pipe->write_ptr;
	} else {
		return (pipe->write_ptr - pipe->read_ptr);
	}
}

size_t pipe_size(fs_node_t * node) {
	pipe_device_t * pipe = (pipe_device_t *)node->inode;
	return pipe_unread(pipe);
}

static inline size_t pipe_available(pipe_device_t * pipe) {
	if (pipe->read_ptr == pipe->write_ptr) {
		return pipe->size - 1;
	}

	if (pipe->read_ptr > pipe->write_ptr) {
		return pipe->read_ptr - pipe->write_ptr - 1;
	} else {
		return (pipe->size - pipe->write_ptr) + pipe->read_ptr - 1;
	}
}

static inline void pipe_increment_read(pipe_device_t * pipe) {
	pipe->read_ptr++;
	if (pipe->read_ptr == pipe->size) {
		pipe->read_ptr = 0;
	}
}

static inline void pipe_increment_write(pipe_device_t * pipe) {
	pipe->write_ptr++;
	if (pipe->write_ptr == pipe->size) {
		pipe->write_ptr = 0;
	}
}

static inline void pipe_increment_write_by(pipe_device_t * pipe, size_t amount) {
	pipe->write_ptr = (pipe->write_ptr + amount) % pipe->size;
}

uint32_t read_pipe(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	//assert(node->inode != 0 && "Attempted to read from a fully-closed pipe.");

	// Retreive the pipe object associated with this file node 
	pipe_device_t * pipe = (pipe_device_t *)node->inode;

	#if DEBUG_PIPES
		if (pipe->size > 300) { //Ignore small pipes (ie, keyboard) 
			print("[debug] Call to read from pipe 0x",-1);
			printHex(node->inode,-1);
			printNewline();
			print("        Unread bytes:    ",-1);
			printDec(pipe_unread(pipe));
			printNewline();
			print("        Total size:      ");
			printDec(pipe->size);
			printNewline();
			print("        Request size:    ");
			printDec(size);
			printNewline();
			print("        Write pointer:   ");
			printDec(pipe->write_ptr);
			printNewline();
			print("        Read  pointer:   ");
			printDec(pipe->read_ptr);
			printNewline();
			print("        Buffer address:  0x");
			printHex(pipe->buffer,-1);
			printNewline();
		}
	#endif

	size_t collected = 0;
	while (collected == 0) {
		lock(&pipe->pi_mutex);
		while (pipe_unread(pipe) > 0 && collected < size) {
			buffer[collected] = pipe->buffer[pipe->read_ptr];
			pipe_increment_read(pipe);
			collected++;
		}
		unlock(&pipe->pi_mutex);
		//wakeup_queue(pipe->wait_queue);
		//switch_from_cross_thread_lock();
		// Deschedule and switch 
		if (collected == 0) {
			//sleep_on(pipe->wait_queue);
		}
	}

	return collected;
}

uint32_t write_pipe(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer) {
	//assert(node->inode != 0 && "Attempted to write to a fully-closed pipe.");

	// Retreive the pipe object associated with this file node 
	pipe_device_t * pipe = (pipe_device_t *)node->inode;

#if DEBUG_PIPES
	if (pipe->size > 300) { // Ignore small pipes (ie, keyboard) 
		print("[debug] Call to write to pipe 0x",-1);
		printHex(node->inode,-1);
		printNewline();
		print("        Available space: ");
		printDec(pipe_available(pipe));
		printNewline();
		print("        Total size:      ");
		printDec(pipe->size);
		printNewline();
		print("        Request size:    ");
		printDec(size);
		printNewline();
		print("        Write pointer:   ");
		printDec(pipe->write_ptr);
		printNewline();
		print("        Read  pointer:   ");
		printDec(pipe->read_ptr);
		printNewline();
		print("        Buffer address:  0x");
		printHex(pipe->buffer,-1);
		printNewline();
		print(" Write: ");
		print(buffer,-1);
		printNewline();
	}
#endif

	size_t written = 0;
	while (written < size) {
		lock(&pipe->pi_mutex);

#if 0
		size_t available = 0;
		if (pipe->read_ptr <= pipe->write_ptr) {
			available = pipe->size - pipe->write_ptr;
		} else {
			available = pipe->read_ptr - pipe->write_ptr - 1;
		}
		if (available) {
			available = min(available, size - written);
			memcpy(&pipe->buffer[pipe->write_ptr], buffer, available);
			pipe_increment_write_by(pipe, available);
			written += available;
		}
#else
		while (pipe_available(pipe) > 0 && written < size) {
			pipe->buffer[pipe->write_ptr] = buffer[written];
			pipe_increment_write(pipe);
			written++;
		}
#endif

		unlock(&pipe->pi_mutex);
		//wakeup_queue(pipe->wait_queue);
		if (written < size) {
			//sleep_on(pipe->wait_queue);
		}
	}

	return written;
}


int wakeup_queue(list_t * queue) {
	int awoken_processes = 0;
	while (queue->length > 0) {
		node_t * node = list_pop(queue);
		if (!((process_t *)node->value)->finished) {
			make_process_ready(node->value);
		}
		awoken_processes++;
	}
	return awoken_processes;
}

int sleep_on(list_t * queue) {
	if (current_process->sleep_node.prev || current_process->sleep_node.next) {
		// uh, we can't sleep right now, we're marked as ready 
		switch_task(0);
		return 0;
	}
	list_append(queue, (node_t *)&current_process->sleep_node);
	switch_task(0);
	return 0;
}	
void make_process_ready(process_t * proc) {
	if (proc->sched_node.prev != NULL || proc->sched_node.next != NULL) // Process is already ready, or someone stole our scheduling node.  
		return;
	list_append(process_queue, &proc->sched_node);
}


void open_pipe(fs_node_t * node, uint8_t read, uint8_t write) {
	//assert(node->inode != 0 && "Attempted to open a fully-closed pipe.");

	// Retreive the pipe object associated with this file node 
	pipe_device_t * pipe = (pipe_device_t *)node->inode;

	// Add a reference 
	pipe->refcount++;

	return;
}

void close_pipe(fs_node_t * node) {
	//assert(node->inode != 0 && "Attempted to close an already fully-closed pipe.");

	// Retreive the pipe object associated with this file node 
	pipe_device_t * pipe = (pipe_device_t *)node->inode;

	// Drop one reference 
	pipe->refcount--;

	// Check the reference count number 
	if (pipe->refcount == 0) {
#if 0
		/// No other references exist, free the pipe (but not its buffer) 
		free(pipe->buffer);
		list_free(pipe->wait_queue);
		free(pipe->wait_queue);
		free(pipe);
		// And let the creator know there are no more references 
		node->inode = 0;
#endif
	}

	return;
}

fs_node_t * make_pipe(size_t size) {
	fs_node_t * fnode = k_malloc(sizeof(fs_node_t));
	pipe_device_t * pipe = k_malloc(sizeof(pipe_device_t));
	mutex *pi_mutexCreate = initLock();
	if (pi_mutexCreate == NULL) 
		return false;

	fnode->inode = 0;
	fnode->name[0] = '\0';
	k_strcpy(fnode->name, "[pipe]");
	fnode->uid   = 0;
	fnode->gid   = 0;
	fnode->flags = FS_PIPE;
	fnode->read  = read_pipe;
	fnode->write = write_pipe;
	fnode->open  = open_pipe;
	fnode->close = close_pipe;
	fnode->inode = (uintptr_t)pipe;

	pipe->buffer    = k_malloc(size);
	pipe->write_ptr = 0;
	pipe->read_ptr  = 0;
	pipe->size      = size;
	pipe->refcount  = 0;
	pipe->pi_mutex  = &pi_mutexCreate;

	pipe->wait_queue = buildList(&equal);

	return fnode;
}*/


	//USERLAND 
	/*
static int mkpipe() {
	fs_node_t * node = make_pipe(4096 * 2);
	return process_append_fd((process_t *)current_process, node);
}*/