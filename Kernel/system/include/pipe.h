
#ifndef __INC_PIPE__
	#define __INC_PIPE__

	#include "../../Common/common.h"
	#include "../include/mutex.h"
	#include "../scheduler/include/processList.h"

/*
	#define FS_PIPE 0x10

	typedef struct _pipe_device {
		uint8_t * buffer;
		size_t write_ptr;
		size_t read_ptr;
		size_t size;
		size_t refcount;
		mutex *pi_mutex;
		list wait_queue;
	} pipe_device_t;

	typedef uint32_t (*write_type_t) (struct fs_node *, uint32_t, uint32_t, uint8_t *);
	typedef void (*open_type_t) (struct fs_node *, uint8_t read, uint8_t write);
	typedef void (*close_type_t) (struct fs_node *);
	typedef uint32_t (*read_type_t) (struct fs_node *, uint32_t, uint32_t, uint8_t *);

	typedef struct fs_node {

		uint32_t mask;			// The permissions mask.
		uint32_t length;		// Size of the file, in byte.
		uint32_t impl;			// Used to keep track which fs it belongs to.

		char name[MAX_BUFFER];			// The filename.
		uint32_t inode;			// Inode number.		
		uint32_t uid;			// The owning user.
		uint32_t gid;			// The owning group.
		uint32_t flags;			// Flags (node type, etc).
		read_type_t read;
		write_type_t write;
		open_type_t open;
		close_type_t close;
		struct fs_node *ptr;	// Used by mountpoints and symlinks.

	} fs_node_t;

	fs_node_t * make_pipe(size_t size);
	size_t pipe_size(fs_node_t * node);

	uint32_t read_pipe(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
	uint32_t write_pipe(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
	void open_pipe(fs_node_t *node, uint8_t read, uint8_t write);
	void close_pipe(fs_node_t *node);*/

#endif