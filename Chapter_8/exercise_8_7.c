#include <stdio.h>

#define NALLOC 1024
#define MAXALLOC (1024*16)

typedef long Align;

union header {
    struct {
        union header *ptr;
        unsigned size;
    } s;
    Align x;
};

typedef union header Header;

static unsigned maxalloc;
static Header base;
static Header *freep = NULL;

void *malloc_c(unsigned);
void free_c(void *);

int main() {
    int i, *p = malloc_c(10 * sizeof(int));

    for(i = 0; i < 10; i++) {
        p[i] = i * 2;
    }

    printf("%d\t%d\n", *p, p[8]);

    free_c(p);

    return 0;
}

static Header *morecore(unsigned nu) {
    char *cp, *sbrk(int);
    Header *up;

    if (nu < NALLOC) {
        nu = NALLOC;
    }
    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *) -1) {
        return NULL;
    }
    up = (Header *) cp;
    up->s.size = nu;
    maxalloc = (nu > maxalloc) ? nu : maxalloc;
    free_c((void *)(up+1));
    return freep;
}

void *malloc_c(unsigned nbytes) {
    Header *p, *prevp;
    Header *morecore(unsigned);
    unsigned nunits;

    if (nbytes <= 0 || nbytes > MAXALLOC) {
        fprintf(stderr, "malloc_c: nbytes %d is invalid\n", nbytes);
        return NULL;
    }

    nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;

    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                prevp->s.ptr = p->s.ptr;
            }
            else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p+1);
        }
        if (p == freep) {
            if ((p = morecore(nunits)) == NULL) {
                return NULL;
            }
        }
    }
}

void free_c(void *ap) {
    Header *bp, *p;

    bp = (Header *)ap - 1;
    
    if (bp->s.size <= 0 || bp->s.size > maxalloc) {
        fprintf(stderr, "free_c: size %d is invalid\n", bp->s.size);
        return;
    }

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) {
            break;
        }
    }

    if (bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    }
    else {
        bp->s.ptr = p->s.ptr;
    }

    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    }
    else {
        p->s.ptr = bp;
    }
    freep = p;
}
