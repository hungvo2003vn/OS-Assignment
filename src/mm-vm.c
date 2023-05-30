//#ifdef MM_PAGING
/*
 * PAGING based Memory Management
 * Virtual memory module mm/mm-vm.c
 */

#include "string.h"
#include "mm.h"
#include <stdlib.h>
#include <stdio.h>

/*enlist_vm_freerg_list - add new rg to freerg_list
 *@mm: memory region
 *@rg_elmt: new region
 *
 */
int enlist_vm_freerg_list(struct mm_struct *mm, struct vm_rg_struct *rg_elmt)
{
  struct vm_rg_struct *rg_node = mm->mmap->vm_freerg_list;

  if (rg_elmt->rg_start >= rg_elmt->rg_end)
    return -1;

  // if (rg_node != NULL)
  //   rg_elmt->rg_next = rg_node;

  // /* Enlist the new region */
  // mm->mmap->vm_freerg_list = rg_elmt;

  /*Create newNode same value as rg_elmt*/
  struct vm_rg_struct *newNode = malloc(sizeof(struct vm_rg_struct));
  newNode->rg_start = rg_elmt->rg_start;
  newNode->rg_end = rg_elmt->rg_end;
  newNode->rg_next = NULL;

  /*enlist at head of linkedlist*/
  if(rg_node == NULL || newNode->rg_end < rg_node->rg_start)
  {
    newNode->rg_next = rg_node;
    mm->mmap->vm_freerg_list = newNode;

    return 0;
  }

  struct vm_rg_struct *curr = mm->mmap->vm_freerg_list;
  struct vm_rg_struct *prev = NULL;
  int merged = 0;

  /*Merging front*/
  while(curr != NULL)
  {
    if(newNode->rg_start > curr->rg_end)
    {
      prev = curr;
      curr = curr->rg_next;
    }

    else if(newNode->rg_end >= curr->rg_start || newNode->rg_start <= curr->rg_end)
    {
      curr->rg_start = (newNode->rg_start < curr->rg_start) ? newNode->rg_start : curr->rg_start;
      curr->rg_end = (newNode->rg_end > curr->rg_end) ? newNode->rg_end : curr->rg_end;

      merged = 1;
      free(newNode);
      break;
    }
    //break;
  }

  /*Merged check*/
  if(!merged)
  {
    prev->rg_next = newNode;
    newNode->rg_next = curr;

    curr = newNode; //Move to the newNode position
  }

  /*Merging behind*/
  struct vm_rg_struct *curr_next = curr->rg_next;
  if(curr_next != NULL && curr_next->rg_start <= curr->rg_end)
  {
    curr->rg_end = (curr_next->rg_end > curr->rg_end) ? curr_next->rg_end : curr->rg_end;

    /*clone curr_next*/
    curr->rg_next = curr_next->rg_next;
    free(curr_next);
  }

  return 0;
}

/*get_vma_by_num - get vm area by numID
 *@mm: memory region
 *@vmaid: ID vm area to alloc memory region
 *
 */
struct vm_area_struct *get_vma_by_num(struct mm_struct *mm, int vmaid)
{
  struct vm_area_struct *pvma= mm->mmap;

  if(mm->mmap == NULL)
    return NULL;

  int vmait = 0;
  
  while (vmait < vmaid)
  {
    if(pvma == NULL)
	  return NULL;

    pvma = pvma->vm_next;
  }

  return pvma;
}

/*get_symrg_byid - get mem region by region ID
 *@mm: memory region
 *@rgid: region ID act as symbol index of variable
 *
 */
struct vm_rg_struct *get_symrg_byid(struct mm_struct *mm, int rgid)
{
  if(rgid < 0 || rgid > PAGING_MAX_SYMTBL_SZ)
    return NULL;

  return &mm->symrgtbl[rgid];
}

/*__alloc - allocate a region memory
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@rgid: memory region ID (used to identify variable in symbole table)
 *@size: allocated size 
 *@alloc_addr: address of allocated memory region
 *
 */
int __alloc(struct pcb_t *caller, int vmaid, int rgid, int size, int *alloc_addr)
{
  
  /*Allocate at the toproof */
  struct vm_rg_struct rgnode;

  if (get_free_vmrg_area(caller, vmaid, size, &rgnode) == 0)
  {
    caller->mm->symrgtbl[rgid].rg_start = rgnode.rg_start;
    caller->mm->symrgtbl[rgid].rg_end = rgnode.rg_end;

    *alloc_addr = rgnode.rg_start;
    
    return 0;
  }
  
  /* TODO get_free_vmrg_area FAILED handle the region management (Fig.6)*/

  /*Attempt to increate limit to get space */
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  //int inc_sz = PAGING_PAGE_ALIGNSZ(size);
  
  int old_sbrk ;
  old_sbrk = cur_vma->sbrk;
  int remain = cur_vma->vm_end - old_sbrk;
  int inc_sz;

  /* TODO INCREASE THE LIMIT
   * inc_vma_limit(caller, vmaid, inc_sz)
   */
  if(size > remain) //not enough space for allocation
  {
    inc_sz = PAGING_PAGE_ALIGNSZ(size - remain);
    if(inc_vma_limit(caller, vmaid, inc_sz) < 0) //overlap or out of mem
      return -1;
  }
  
  /*Successful increase limit */
  caller->mm->symrgtbl[rgid].rg_start = old_sbrk;
  caller->mm->symrgtbl[rgid].rg_end = old_sbrk + size;

  *alloc_addr = old_sbrk;
  cur_vma->sbrk += size;
  
  return 0;
}

/*__free - remove a region memory
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@rgid: memory region ID (used to identify variable in symbole table)
 *@size: allocated size 
 *
 */
int __free(struct pcb_t *caller, int vmaid, int rgid)
{
  struct vm_rg_struct *rgnode;

  if(rgid < 0 || rgid > PAGING_MAX_SYMTBL_SZ)
    return -1;

  /* TODO: Manage the collect freed region to freerg_list */
  rgnode = get_symrg_byid(caller->mm, rgid);
  
  /*enlist the obsoleted memory region */
  enlist_vm_freerg_list(caller->mm, rgnode);

  printf("[PID: %d]----After enlist----\n", caller->pid);
  print_list_rg(caller->mm->mmap->vm_freerg_list);

  return 0;
}

/*pgalloc - PAGING-based allocate a region memory
 *@proc:  Process executing the instruction
 *@size: allocated size 
 *@reg_index: memory region ID (used to identify variable in symbole table)
 */
int pgalloc(struct pcb_t *proc, uint32_t size, uint32_t reg_index)
{
  int addr;

  /* By default using vmaid = 0 */
  return __alloc(proc, 0, reg_index, size, &addr);
}

/*pgfree - PAGING-based free a region memory
 *@proc: Process executing the instruction
 *@size: allocated size 
 *@reg_index: memory region ID (used to identify variable in symbole table)
 */

int pgfree_data(struct pcb_t *proc, uint32_t reg_index)
{
   return __free(proc, 0, reg_index);
}

/*pg_getpage - get the page in ram
 *@mm: memory region
 *@pagenum: PGN
 *@framenum: return FPN
 *@caller: caller
 *
 */
int pg_getpage(struct mm_struct *mm, int pgn, int *fpn, struct pcb_t *caller)
{
  uint32_t pte = mm->pgd[pgn];

  if (!PAGING_PAGE_PRESENT(pte))
  { /* Page is not online, make it actively living */
    struct pgn_t *fifo_node = malloc(sizeof(struct pgn_t));
    int swpfpn; 

    int tgtfpn = PAGING_SWP(pte);//the target frame storing our variable
    printf("-----[PID: %d] PAGE FAULT----- target page in swap: %08x\n", caller->pid, mm->pgd[pgn]);

    /* TODO: Play with your paging theory here */
    /* Find victim page in virtual mem */
    if(find_victim_page(caller->mram, &fifo_node) < 0)
    {
#ifdef MMDBG
      printf("-----[PID: %d] PAGE FAULT----- pg_getpage failed - no victim to swapoff\n", caller->pid);
#endif
      return -1;
    }

    printf("-----[PID: %d] PAGE FAULT----- found victim: %08x\n", caller->pid, *fifo_node->pgd_pgn);

    /*victim in ram*/
    int ram_vicpgn = PAGING_FPN(*fifo_node->pgd_pgn);

    /* Get free frame in MEMSWP */
    if(MEMPHY_get_freefp(caller->active_mswp, &swpfpn) < 0) return -1;


    /* Do swap frame from MEMRAM to MEMSWP and vice versa*/
    /* Copy victim frame to swap */
    __swap_cp_page(caller->mram, ram_vicpgn, caller->active_mswp, swpfpn);

    /* Copy target frame from swap to mem */
    __swap_cp_page(caller->active_mswp, tgtfpn, caller->mram, ram_vicpgn);

    /* Make the tgtfpn in activeswap free again*/
    MEMPHY_put_freefp(caller->active_mswp, tgtfpn);

    /* Update page table */
    pte_set_swap(fifo_node->pgd_pgn, 0, swpfpn);
    printf("-----[PID: %d] PAGE FAULT----- victim_present: %08x\n", caller->pid, *fifo_node->pgd_pgn);
    free(fifo_node); //avoid mem leak

    /* Update its online status of the target page */
    pte_set_fpn(&pte, ram_vicpgn);
    mm->pgd[pgn] = pte;
    
    printf("-----[PID: %d] PAGE FAULT----- target_put: %08x\n", caller->pid, mm->pgd[pgn]);
    enlist_pgn_node(caller->mram, pgn, &mm->pgd[pgn]);
  }

  *fpn = PAGING_FPN(pte);

  return 0;
}

/*pg_getval - read value at given offset
 *@mm: memory region
 *@addr: virtual address to acess 
 *@value: value
 *
 */
int pg_getval(struct mm_struct *mm, int addr, BYTE *data, struct pcb_t *caller)
{
  int pgn = PAGING_PGN(addr);
  int off = PAGING_OFFST(addr);
  int fpn;

  /* Get the page to MEMRAM, swap from MEMSWAP if needed */
  if(pg_getpage(mm, pgn, &fpn, caller) != 0) 
    return -1; /* invalid page access */

  int phyaddr = (fpn << PAGING_ADDR_FPN_LOBIT) + off;

  MEMPHY_read(caller->mram,phyaddr, data);

  return 0;
}

/*pg_setval - write value to given offset
 *@mm: memory region
 *@addr: virtual address to acess 
 *@value: value
 *
 */
int pg_setval(struct mm_struct *mm, int addr, BYTE value, struct pcb_t *caller)
{
  int pgn = PAGING_PGN(addr);
  int off = PAGING_OFFST(addr);
  int fpn;

  /* Get the page to MEMRAM, swap from MEMSWAP if needed */
  if(pg_getpage(mm, pgn, &fpn, caller) != 0) 
    return -1; /* invalid page access */

  int phyaddr = (fpn << PAGING_ADDR_FPN_LOBIT) + off;

  MEMPHY_write(caller->mram,phyaddr, value);

   return 0;
}

/*__read - read value in region memory
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@offset: offset to acess in memory region 
 *@rgid: memory region ID (used to identify variable in symbole table)
 *@size: allocated size 
 *
 */
int __read(struct pcb_t *caller, int vmaid, int rgid, int offset, BYTE *data)
{
  struct vm_rg_struct *currg = get_symrg_byid(caller->mm, rgid);

  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  if(currg == NULL || cur_vma == NULL) /* Invalid memory identify */
	  return -1;

  if(currg->rg_start + offset > currg->rg_end) /* Invalid offset*/
    return -1;

  pg_getval(caller->mm, currg->rg_start + offset, data, caller);

  return 0;
}


/*pgwrite - PAGING-based read a region memory */
int pgread(
		struct pcb_t * proc, // Process executing the instruction
		uint32_t source, // Index of source register
		uint32_t offset, // Source address = [source] + [offset]
		uint32_t destination) 
{
  BYTE data;
  int val = __read(proc, 0, source, offset, &data);

  destination = (uint32_t) data;
#ifdef IODUMP
  if(val == 0)
    printf("[PID: %d] read region=%d offset=%d value=%d\n", proc->pid,source, offset, data);
  else
    printf("[PID: %d] read region=%d offset=%d: INVALID REGION OFFSET\n", proc->pid,source, offset);
#ifdef PAGETBL_DUMP
  print_pgtbl(proc, 0, -1); //print max TBL
#endif
  MEMPHY_dump(proc->mram);
#endif

  return val;
}

/*__write - write a region memory
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@offset: offset to acess in memory region 
 *@rgid: memory region ID (used to identify variable in symbole table)
 *@size: allocated size 
 *
 */
int __write(struct pcb_t *caller, int vmaid, int rgid, int offset, BYTE value)
{
  struct vm_rg_struct *currg = get_symrg_byid(caller->mm, rgid);

  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);
  
  if(currg == NULL || cur_vma == NULL) /* Invalid memory identify */
	  return -1;

  if(currg->rg_start + offset > currg->rg_end) /* Invalid offset*/
    return -1;

  pg_setval(caller->mm, currg->rg_start + offset, value, caller);
  
  return 0;
}

/*pgwrite - PAGING-based write a region memory */
int pgwrite(
		struct pcb_t * proc, // Process executing the instruction
		BYTE data, // Data to be wrttien into memory
		uint32_t destination, // Index of destination register
		uint32_t offset)
{
#ifdef IODUMP
  printf("[PID: %d] write region=%d offset=%d value=%d\n", proc->pid, destination, offset, data);
#ifdef PAGETBL_DUMP
  print_pgtbl(proc, 0, -1); //print max TBL
#endif
  MEMPHY_dump(proc->mram);
#endif

  return __write(proc, 0, destination, offset, data);
}


/*free_pcb_memphy - collect all memphy of pcb
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@incpgnum: number of page
 */
int free_pcb_memph(struct pcb_t *caller)
{
  int pagenum, fpn;
  uint32_t pte;


  for(pagenum = 0; pagenum < PAGING_MAX_PGN; pagenum++)
  {
    pte= caller->mm->pgd[pagenum];

    if (PAGING_PAGE_PRESENT(pte))
    {
      fpn = PAGING_FPN(pte);
      MEMPHY_put_freefp(caller->mram, fpn);
    } else {
      fpn = PAGING_SWP(pte);
      MEMPHY_put_freefp(caller->active_mswp, fpn);    
    }
  }

  return 0;
}

/*get_vm_area_node - get vm area for a number of pages
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@incpgnum: number of page
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
struct vm_rg_struct* get_vm_area_node_at_brk(struct pcb_t *caller, int vmaid, int size, int alignedsz)
{
  struct vm_rg_struct * newrg;
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  newrg = malloc(sizeof(struct vm_rg_struct));

  newrg->rg_start = cur_vma->sbrk;
  newrg->rg_end = newrg->rg_start + size;

  return newrg;
}

/*validate_overlap_vm_area
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@vmastart: vma end
 *@vmaend: vma end
 *
 */
int validate_overlap_vm_area(struct pcb_t *caller, int vmaid, int vmastart, int vmaend)
{

  /* TODO validate the planned memory area is not overlapped */
  struct vm_area_struct *cur_vma = caller->mm->mmap;

  return OVERLAP(cur_vma->vm_start, cur_vma->vm_end, vmastart, vmaend)?1:-1;
}

/*inc_vma_limit - increase vm area limits to reserve space for new variable
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@inc_sz: increment size 
 *
 */
int inc_vma_limit(struct pcb_t *caller, int vmaid, int inc_sz)
{
  struct vm_rg_struct * newrg = malloc(sizeof(struct vm_rg_struct));
  int inc_amt = PAGING_PAGE_ALIGNSZ(inc_sz);
  int incnumpage =  inc_amt / PAGING_PAGESZ;
  struct vm_rg_struct *area = get_vm_area_node_at_brk(caller, vmaid, inc_sz, inc_amt);
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  int old_end = cur_vma->vm_end;

  /*Validate overlap of obtained region */
  if (validate_overlap_vm_area(caller, vmaid, area->rg_start, area->rg_end) < 0){
    free(area); //avoid mem leak
    return -1; /*Overlap and failed allocation */
  }

  /* The obtained vm area (only) 
   * now will be alloc real ram region */
  cur_vma->vm_end += inc_sz;
  if (vm_map_ram(caller, area->rg_start, area->rg_end, 
                          old_end, incnumpage , newrg) < 0)
  {
    free(area); //avoid mem leak
    return -1; /* Map the memory to MEMRAM */
  }
  
  free(area); //avoid mem leak
  return 0;

}

/*find_victim_page - find victim page
 *@caller: caller
 *@pgn: return page number
 *
 */
int find_victim_page(struct memphy_struct *mp, struct pgn_t **fifo_node)
{
  pthread_mutex_lock(&mp->fifo_lock);
  struct pgn_t *pg = mp->fifo_pgn;

  /* TODO: Implement the theorical mechanism to find the victim page */
  struct pgn_t *pre_pg = NULL; 

  if(pg == NULL) return -1; //RAM is busy

  while(pg->pg_next != NULL){
    pre_pg = pg;
    pg = pg->pg_next;
  }

  (*fifo_node)->pgd_pgn = pg->pgd_pgn; //victim in FIFO is the end of the linked list
  (*fifo_node)->pgn = pg->pgn;
  free(pg);

  if(pre_pg != NULL) pre_pg->pg_next = NULL; // set current end to NULL
  else mp->fifo_pgn = NULL; //no pre_pg case
  
  pthread_mutex_unlock(&mp->fifo_lock);
  return 0;
}

/*get_free_vmrg_area - get a free vm region
 *@caller: caller
 *@vmaid: ID vm area to alloc memory region
 *@size: allocated size 
 *
 */
int get_free_vmrg_area(struct pcb_t *caller, int vmaid, int size, struct vm_rg_struct *newrg)
{
  struct vm_area_struct *cur_vma = get_vma_by_num(caller->mm, vmaid);

  struct vm_rg_struct *rgit = cur_vma->vm_freerg_list;

  if (rgit == NULL)
    return -1;

  /* Probe unintialized newrg */
  newrg->rg_start = newrg->rg_end = -1;

  /* Traverse on list of free vm region to find a fit space */
  while (rgit != NULL)
  {
    if (rgit->rg_start + size <= rgit->rg_end)
    { /* Current region has enough space */
      newrg->rg_start = rgit->rg_start;
      newrg->rg_end = rgit->rg_start + size;

      /* Update left space in chosen region */
      if (rgit->rg_start + size < rgit->rg_end)
      {
        rgit->rg_start = rgit->rg_start + size;
      }
      else
      { /*Use up all space, remove current node */
        /*Clone next rg node */
        struct vm_rg_struct *nextrg = rgit->rg_next;

        /*Cloning */
        if (nextrg != NULL)
        {
          rgit->rg_start = nextrg->rg_start;
          rgit->rg_end = nextrg->rg_end;

          rgit->rg_next = nextrg->rg_next;

          free(nextrg);
        }
        else
        { /*End of free list */
          rgit->rg_start = rgit->rg_end;	//dummy, size 0 region
          rgit->rg_next = NULL;
        }
      }
      break; //Found free region
    }
    else
    {
      rgit = rgit->rg_next;	// Traverse next rg
    }
  }

 if(newrg->rg_start == -1) // new region not found
   return -1;

 return 0;
}

//#endif
