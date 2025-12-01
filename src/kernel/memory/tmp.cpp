#include <cstdint>

using uint32 = uint32_t;
using uint8 = uint8_t;
using uintptr = uintptr_t;

typedef struct _KHEAPBLOCKBM {
  struct _KHEAPBLOCKBM* next;
  uint32 size;
  uint32 used;
  uint32 bsize;
  uint32 lfb;
} HeapBlock;

typedef struct _KHEAPBM {
  HeapBlock* fblock;
} BmHeap;

void k_heapBMInit(BmHeap* heap) {
  heap->fblock = 0;
}

int k_heapBMAddBlock(BmHeap* heap, uintptr addr, uint32 size, uint32 bsize) {
  HeapBlock* b;
  uint32 bcnt;
  uint32 x;
  uint8* bm;

  b = (HeapBlock*)addr;
  b->size = size - sizeof(HeapBlock);
  b->bsize = bsize;

  b->next = heap->fblock;
  heap->fblock = b;

  bcnt = b->size / b->bsize;
  bm = (uint8*)&b[1];

  /* clear bitmap */
  for (x = 0; x < bcnt; ++x) {
    bm[x] = 0;
  }

  /* reserve room for bitmap */
  bcnt = (bcnt / bsize) * bsize < bcnt ? bcnt / bsize + 1 : bcnt / bsize;
  for (x = 0; x < bcnt; ++x) {
    bm[x] = 5;
  }

  b->lfb = bcnt - 1;

  b->used = bcnt;

  return 1;
}

static uint8 k_heapBMGetNID(uint8 a, uint8 b) {
  uint8 c;
  for (c = a + 1; c == b || c == 0; ++c)
    ;
  return c;
}

void* k_heapBMAlloc(BmHeap* heap, uint32 size) {
  HeapBlock* b;
  uint8* bm;
  uint32 bcnt;
  uint32 x, y, z;
  uint32 bneed;
  uint8 nid;

  /* iterate blocks */
  for (b = heap->fblock; b; b = b->next) {
    /* check if block has enough room */
    if (b->size - (b->used * b->bsize) >= size) {

      bcnt = b->size / b->bsize;
      bneed = (size / b->bsize) * b->bsize < size ? size / b->bsize + 1 : size / b->bsize;
      bm = (uint8*)&b[1];

      for (x = (b->lfb + 1 >= bcnt ? 0 : b->lfb + 1); x < b->lfb; ++x) {
        /* just wrap around */
        if (x >= bcnt) { x = 0; }

        if (bm[x] == 0) {
          /* count free blocks */
          for (y = 0; bm[x + y] == 0 && y < bneed && (x + y) < bcnt; ++y)
            ;

          /* we have enough, now allocate them */
          if (y == bneed) {
            /* find ID that does not match left or right */
            nid = k_heapBMGetNID(bm[x - 1], bm[x + y]);

            /* allocate by setting id */
            for (z = 0; z < y; ++z) {
              bm[x + z] = nid;
            }

            /* optimization */
            b->lfb = (x + bneed) - 2;

            /* count used blocks NOT bytes */
            b->used += y;

            return (void*)(x * b->bsize + (uintptr)&b[1]);
          }

          /* x will be incremented by one ONCE more in our FOR loop */
          x += (y - 1);
          continue;
        }
      }
    }
  }

  return 0;
}

void k_heapBMFree(BmHeap* heap, void* ptr) {
  HeapBlock* b;
  uintptr ptroff;
  uint32 bi, x;
  uint8* bm;
  uint8 id;
  uint32 max;

  for (b = heap->fblock; b; b = b->next) {
    if ((uintptr)ptr > (uintptr)b &&
        (uintptr)ptr < (uintptr)b + sizeof(HeapBlock) + b->size) {
      /* found block */
      ptroff = (uintptr)ptr - (uintptr)&b[1]; /* get offset to get block */
      /* block offset in BM */
      bi = ptroff / b->bsize;
      /* .. */
      bm = (uint8*)&b[1];
      /* clear allocation */
      id = bm[bi];
      /* oddly.. GCC did not optimize this */
      max = b->size / b->bsize;
      for (x = bi; bm[x] == id && x < max; ++x) {
        bm[x] = 0;
      }
      /* update free block count */
      b->used -= x - bi;
      return;
    }
  }

  /* this error needs to be raised or reported somehow */
  return;
}
