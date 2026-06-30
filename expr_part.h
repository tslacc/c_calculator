#ifndef EXPR_PART_
#define EXPR_PART_
struct Part{
	char operator;
	long value;
	struct Part *next;	
};
static void cleanup(struct Part *p){
	if(p->next!=NULL){
		cleanup(p->next);
	}
	free(p);
	return;
}

const struct {
	void (*cleanup)(struct Part *p);
} EXPR_P = {cleanup};
#endif