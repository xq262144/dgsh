#ifndef DGSH_NEGOTIATE_H
#define DGSH_NEGOTIATE_H

/* Negotiation protocol states */
enum prot_state {
	PS_COMPLETE,		/* Negotiation process is complete */
	PS_NEGOTIATION,		/* Negotiation phase */
	PS_NEGOTIATION_END,	/* End of negotiation phase */
	PS_RUN,			/* Share solution; prepare to run */
	PS_ERROR,		/* Error in negotiation process */
};

#include <sys/socket.h> /* struct cmsghdr */
union fdmsg {
	struct cmsghdr h;
	char buf[CMSG_SPACE(sizeof(int))];
};

#ifdef UNIT_TESTING
#define STATIC
#else
#define STATIC static
#endif

/**
 * Each tool in the dgsh graph calls dgsh_negotiate() to take part in
 * peer-to-peer negotiation. A message block (MB) is circulated among tools
 * and is filled with tools' I/O requirements. When all requirements are in
 * place, an algorithm runs that tries to find a solution that satisfies
 * all requirements. If a solution is found, pipes are allocated and
 * set up according to the solution. The appropriate file descriptors
 * are provided to each tool and the negotiation phase ends.
 * The function's return value signifies success or failure of the
 * negotiation phase.
 */
enum prot_state
dgsh_negotiate(const char *tool_name, /* Input variable: the program's name */
                    int *n_input_fds, /* Input/Output variable:
				       * number of input file descriptors
				       * required. The number may be changed
				       * by the API and will reflect the size
				       * of the input file descriptor array.
				       * If NULL is provided, then 0 or 1
				       * is implied and no file descriptor
				       * array is returned. The input file
				       * descriptor to return (in case of 1)
				       * substitutes stdin.
				       */
                    int *n_output_fds,/* Input/Output variable:
				       * number of output file descriptors
				       * provided. The semantics for n_input_fds
				       * apply here respectively.
				       */
                    int **input_fds,  /* Output variable:
				       * input file descriptor array
				       * The caller has the responsbility
				       * to free the array.
				       */
                    int **output_fds);/* Output variable:
				       * output file descriptor array
				       * The caller has the responsbility
				       * to free the array.
				       */

/*
 * Results of operations
 * Also negative values signify a failed operation's errno value
 */
enum op_result {
	OP_SUCCESS,		/* Successful */
	OP_ERROR,		/* Unresolvable error due to I/O problem
				 * constraints provided by the processes
				 * on the dgsh graph or memory constraints
				 * of the systems.
				 */
	OP_EXISTS,		/* Node or edge already registered with the
				 * dgsh graph.
				 */
	OP_CREATE,		/* Node ar edge registered with the dgsh 
				 * graph.
				 */
	OP_NOOP,		/* No operation when trying to add an edge
				 * on a graph with just one node at the time.
				 */
	OP_RETRY,		/* Not all constraints of an I/O constraint
				 * problem have been satisfied yet.
				 * Retry by leveraging flexible constraints.
				 */
};

#ifdef UNIT_TESTING

/* Models an I/O connection between tools on an dgsh graph. */
struct dgsh_edge {
        int from; /* Index of node on the graph where data comes from (out). */
        int to; /* Index of node on the graph that receives the data (in). */
        int instances; /* Number of instances of an edge. */
	int from_instances; /* Number of instances the origin node of an edge can provide. */
	int to_instances; /* Number of instances the destination of an edge can require. */
};

#endif

#endif
