# Multilevel Queue Scheduling and Memory Management with Paging

## Introduction
This project aims to explore the concept of Multilevel Queue scheduling and its relationship with RAM memory management using the paging mechanism. Multilevel Queue scheduling is a technique used in operating systems to prioritize and allocate resources to processes based on predefined queues. By combining this scheduling algorithm with memory management through paging, we can efficiently manage system resources and improve overall performance.

## Multilevel Queue Scheduling
Multilevel Queue scheduling divides processes into multiple queues, each with a different priority level. Processes with higher priority levels receive preferential treatment and are scheduled before lower priority processes. This approach ensures that critical processes are executed promptly and efficiently. In our implementation, we can define the number of queues and assign priorities to processes accordingly.

## Memory Management with Paging
Memory management plays a crucial role in optimizing system performance. Paging is a memory management mechanism that allows the operating system to divide physical memory into fixed-sized blocks called pages. Each page is then mapped to corresponding logical addresses, simplifying memory allocation and deallocation.

When combining Multilevel Queue scheduling with paging, we can associate specific memory pages with each queue. This association enables efficient management of memory resources based on process priorities. Higher priority processes can be assigned a larger number of pages to ensure they have sufficient memory to execute their tasks promptly.
### Memory struct
![Paging_Model_latest](https://github.com/hungvo2003vn/OS-Assignment/assets/108314498/4291cf88-784d-438d-9a81-400a9b09b15d)
### Memory model
![Paging Mem](https://github.com/hungvo2003vn/OS-Assignment/assets/108314498/4743e42f-b993-451c-8b1a-4fc4d102590f)
### Optimize allocation
![Optimize allocation](https://github.com/hungvo2003vn/OS-Assignment/assets/108314498/7c044444-aa49-42bd-943f-b22ce5a79cce)


## How It Works
1. Initially, define the number of priority queues and allocate the required memory for each queue.
2. Implement the Multilevel Queue scheduling algorithm to prioritize and assign processes to the appropriate queues.
3. Utilize the paging mechanism to manage memory allocation and mapping for each process.
4. Whenever a process is selected for execution from the highest priority queue, allocate a sufficient number of pages from the corresponding memory block.
5. Execute the process, and upon completion, release the allocated memory pages for reuse.
6. Repeat the scheduling and memory management process until all processes are completed.

## Usage and Configuration
To use the Multilevel Queue scheduling algorithm with memory management by paging, follow these steps:

1. Clone the repository and navigate to the project directory.
2. Configure the number of priority queues in the `os-cfg.h` file.
3. Define the memory allocation for each queue in the `os-mm.h` file.
4. Compile and run the program using the provided build script or commands in `Makefile`.</br>
  - Using the command `make sched` to run only the scheduling part. 
  - Using the command `make mem` to run only the memory part.
  - `make all` or `make` to run all the project
5. Monitor the execution to observe the scheduling behavior and memory management.
  - `Usage: ./sched [configuration name in path input/only_scheduling/]`
  - `Usage: ./mem [name of process file] [configuration name in path input/only_paging/]`
  - `Usage: ./os [path to configure file]`

## Example input/output
### Scheduling
- Input: 

  Configuration: `sched_0`
  
  ```
  2 1 2
  0 s0
  4 s1
  ```
  Process:
  | Process | s0 | s1 |
  |:--------|:---|:---|
  | Description | 12 15<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br><br> | 20 7<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br><br><br><br><br><br><br><br><br><br> |
- Output: `./sched sched_0`

  ```
  Time slot   0
  ld_routine
          Loaded a process at input/proc/s0, PID: 1 PRIO: 4 at time 0
  Time slot   1
          CPU 0: Dispatched process  1 at time 1
          Loaded a process at input/proc/s0, PID: 2 PRIO: 0 at time 1
  Time slot   2
  Time slot   3
          CPU 0: Put process  1 to run queue at time 3
          CPU 0: Dispatched process  2 at time 3
  Time slot   4
  Time slot   5
          CPU 0: Put process  2 to run queue at time 5
          CPU 0: Dispatched process  2 at time 5
  Time slot   6
  Time slot   7
          CPU 0: Put process  2 to run queue at time 7
          CPU 0: Dispatched process  2 at time 7
  Time slot   8
  Time slot   9
          CPU 0: Put process  2 to run queue at time 9
          CPU 0: Dispatched process  2 at time 9
  Time slot  10
  Time slot  11
          CPU 0: Put process  2 to run queue at time 11
          CPU 0: Dispatched process  2 at time 11
  Time slot  12
  Time slot  13
          CPU 0: Put process  2 to run queue at time 13
          CPU 0: Dispatched process  2 at time 13
  Time slot  14
  Time slot  15
          CPU 0: Put process  2 to run queue at time 15
          CPU 0: Dispatched process  2 at time 15
  Time slot  16
  Time slot  17
          CPU 0: Put process  2 to run queue at time 17
          CPU 0: Dispatched process  2 at time 17
  Time slot  18
          CPU 0: Processed  2 has finished at time 18
          CPU 0: Dispatched process  1 at time 18
  Time slot  19
  Time slot  20
          CPU 0: Put process  1 to run queue at time 20
          CPU 0: Dispatched process  1 at time 20
  Time slot  21
  Time slot  22
          CPU 0: Put process  1 to run queue at time 22
          CPU 0: Dispatched process  1 at time 22
  Time slot  23
  Time slot  24
          CPU 0: Put process  1 to run queue at time 24
          CPU 0: Dispatched process  1 at time 24
  Time slot  25
  Time slot  26
          CPU 0: Put process  1 to run queue at time 26
          CPU 0: Dispatched process  1 at time 26
  Time slot  27
  Time slot  28
          CPU 0: Put process  1 to run queue at time 28
          CPU 0: Dispatched process  1 at time 28
  Time slot  29
  Time slot  30
          CPU 0: Put process  1 to run queue at time 30
          CPU 0: Dispatched process  1 at time 30
  Time slot  31
          CPU 0: Processed  1 has finished at time 31
          CPU 0 stopped at time 31
  
  ```
### Memory Management
- Input:

  Process: `m0s`
  
  ```
  1 7
  alloc 300 0
  alloc 100 1
  free 0
  alloc 100 2
  write 102 1 20
  write 1 0 40
  read 0 40 40
  ```
  Configuration: `paging_1KB`
  
  ```
  1024 16777216 0 0 0
  ```
- Output: `./mem m0s paging_1KB`

  ```
  configPath: input/only_paging/paging_1KB
  [PID: 1]----After enlist----
  print_list_rg: 
  rg[0->300]

  [PID: 1] write region=1 offset=20 value=102
  print_list_using_fpn_RAM: 
  ra[80000000]-
  ra[80000001]-

  print_pgtbl: 0 - 512
  00000000: 80000001
  00000004: 80000000
  Memory content [addr]-[offset,value]: | max size = 1024

  [PID: 1] write region=0 offset=40 value=1
  print_list_using_fpn_RAM: 
  ra[80000000]-
  ra[80000001]-

  print_pgtbl: 0 - 512
  00000000: 80000001
  00000004: 80000000
  Memory content [addr]-[offset,value]: 
                 [80000000]-[64,102]| max size = 1024

  [PID: 1] read region=0 offset=40 value=1
  print_list_using_fpn_RAM: 
  ra[80000000]-
  ra[80000001]-

  print_pgtbl: 0 - 512
  00000000: 80000001
  00000004: 80000000
  Memory content [addr]-[offset,value]: 
                 [80000000]-[64,102]
                 [80000001]-[40,1]| max size = 1024
                 
  ```
### Put it all together
- Input:

  Configuration: `os_1_mlq_paging_small_1KB`
  
  ```
  2 4 8
  1024 16777216 0 0 0
  1 p0s  130
  2 s3  39
  4 m1s  15
  6 s2  120
  7 m0s  120
  9 p1s  15
  11 s0 38
  16 s1 0
  ```
  Process:
  | Process | p0s | s3 | m1s | s2 | m0s | p1s | s0 | s1 |
  |:--------|:----|:---|:----|:---|:----|:----|:---|:---|
  | Description | 1 14<br>calc<br>alloc 300 0<br>alloc 300 4<br>free 0<br>alloc 100 1<br>write 100 1 20<br>read 1 20 20<br>write 102 2 20<br>read 2 20 20<br>write 103 3 20<br>read 3 20 20<br>calc<br>free 4<br>calc<br><br><br> | 7 17<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc | 1 6<br>alloc 300 0<br>alloc 100 1<br>free 0<br>alloc 100 2<br>free 2<br>free 1<br><br><br><br><br><br><br><br><br><br><br> | 20 13<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br><br><br><br> | 1 7<br>alloc 300 0<br>alloc 100 1<br>free 0<br>alloc 100 2<br>write 102 1 20<br>write 1 0 40<br>read 0 40 40<br><br><br><br><br><br><br><br><br><br> | 1 11<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br><br><br><br><br><br> | 12 15<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br><br> | 20 7<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br>calc<br><br><br><br><br><br><br><br><br><br> |            
- Output: `./os os_1_mlq_paging_small_1KB`

  ```
  Time slot   0
  ld_routine
          Loaded a process at input/proc/p0s, PID: 1 PRIO: 130 at time 1
          CPU 2: Dispatched process  1 at time 1
  Time slot   1
  Time slot   2
          Loaded a process at input/proc/s3, PID: 2 PRIO: 39 at time 2
          CPU 3: Dispatched process  2 at time 3
          CPU 2: Put process  1 to run queue at time 3
          CPU 2: Dispatched process  1 at time 3
  Time slot   3
          Loaded a process at input/proc/m1s, PID: 3 PRIO: 15 at time 4
  [PID: 1]----After enlist----
  print_list_rg: 
  rg[0->300]

  Time slot   4
          CPU 1: Dispatched process  3 at time 4
  -----[PID: 3] victim_OOR: 80000001
          CPU 3: Put process  2 to run queue at time 5
  Time slot   5
          CPU 3: Dispatched process  2 at time 5
          CPU 2: Put process  1 to run queue at time 5
          CPU 2: Dispatched process  1 at time 5
          Loaded a process at input/proc/s2, PID: 4 PRIO: 120 at time 6
  [PID: 1] write region=1 offset=20 value=100
  print_list_using_fpn_RAM: 
  ra[80000003]-
  ra[80000001]-
  ra[80000002]-
  ra[80000000]-

  print_pgtbl: 0 - 768
  00000000: 40000000
  00000004: 80000000
  00000008: 80000002
  Time slot   6
          CPU 0: Dispatched process  4 at time 6
          CPU 1: Put process  3 to run queue at time 6
          CPU 1: Dispatched process  3 at time 6
  [PID: 3]----After enlist----
  print_list_rg:
  rg[0->300]

  Memory content [addr]-[offset,value]: | max size = 1024

  -----[PID: 1] PAGE FAULT----- target page in swap: 40000000
  -----[PID: 1] PAGE FAULT----- found victim: 80000000
  -----[PID: 1] PAGE FAULT----- victim_present: 40000020
  -----[PID: 1] PAGE FAULT----- target_put: 80000000
          CPU 3: Put process  2 to run queue at time 7
          CPU 3: Dispatched process  2 at time 7
  Time slot   7
          CPU 2: Put process  1 to run queue at time 7
          CPU 2: Dispatched process  1 at time 7
  [PID: 1] read region=1 offset=20 value=100
  print_list_using_fpn_RAM:
  ra[80000000]-
  ra[80000003]-
  ra[80000001]-
  ra[80000002]-

  print_pgtbl: 0 - 768
  00000000: 80000000
  00000004: 40000020
  00000008: 80000002
  Memory content [addr]-[offset,value]:
                 [80000000]-[20,100]| max size = 1024

          Loaded a process at input/proc/m0s, PID: 5 PRIO: 120 at time 7
  [PID: 1] write region=2 offset=20 value=102
  print_list_using_fpn_RAM:
  Time slot   8
  ra[80000000]-
  ra[80000003]-
  ra[80000001]-
  ra[80000002]-

  print_pgtbl: 0 - 768
  00000000: 80000000
  00000004: 40000020
  00000008: 80000002
          CPU 1: Put process  3 to run queue at time 8
          CPU 1: Dispatched process  3 at time 8
  [PID: 3]----After enlist----
  print_list_rg:
  rg[0->300]

  Memory content [addr]-[offset,value]:
                 [80000000]-[20,100]| max size = 1024

          CPU 0: Put process  4 to run queue at time 8
          CPU 0: Dispatched process  5 at time 8
  -----[PID: 5] victim_OOR: 80000002
  -----[PID: 5] victim_OOR: 80000001
          Loaded a process at input/proc/p1s, PID: 6 PRIO: 15 at time 9
  Time slot   9
          CPU 2: Put process  1 to run queue at time 9
          CPU 2: Dispatched process  6 at time 9
          CPU 3: Put process  2 to run queue at time 9
          CPU 3: Dispatched process  2 at time 9
  [PID: 3]----After enlist----
  print_list_rg:
  rg[0->400]

          CPU 1: Processed  3 has finished at time 10
          CPU 1: Dispatched process  4 at time 10
  Time slot  10
          CPU 0: Put process  5 to run queue at time 10
          CPU 0: Dispatched process  5 at time 10
  [PID: 5]----After enlist----
  print_list_rg:
  rg[0->300]

          Loaded a process at input/proc/s0, PID: 7 PRIO: 38 at time 11
          CPU 3: Put process  2 to run queue at time 11
          CPU 3: Dispatched process  7 at time 11
          CPU 2: Put process  6 to run queue at time 11
          CPU 2: Dispatched process  6 at time 11
  Time slot  11
          CPU 1: Put process  4 to run queue at time 12
          CPU 1: Dispatched process  2 at time 12
          CPU 0: Put process  5 to run queue at time 12
          CPU 0: Dispatched process  5 at time 12
  [PID: 5] write region=1 offset=20 value=102
  print_list_using_fpn_RAM:
  ra[80000002]-
  ra[80000001]-
  ra[80000000]-
  Time slot  12
  ra[80000003]-

  print_pgtbl: 0 - 512
  00000000: 80000001
  00000004: 80000002
  Memory content [addr]-[offset,value]:
                 [80000000]-[20,100]| max size = 1024

          CPU 3: Put process  7 to run queue at time 13
          CPU 3: Dispatched process  7 at time 13
  Time slot  13
          CPU 2: Put process  6 to run queue at time 13
          CPU 2: Dispatched process  6 at time 13
  [PID: 5] write region=0 offset=40 value=1
  print_list_using_fpn_RAM:
  ra[80000002]-
  ra[80000001]-
  ra[80000000]-
  ra[80000003]-

  print_pgtbl: 0 - 512
  00000000: 80000001
  00000004: 80000002
  Memory content [addr]-[offset,value]:
                 [80000000]-[20,100]
                 [80000002]-[64,102]| max size = 1024

          CPU 1: Put process  2 to run queue at time 14
          CPU 1: Dispatched process  2 at time 14
          CPU 0: Put process  5 to run queue at time 14
          CPU 0: Dispatched process  5 at time 14
  [PID: 5] read region=0 offset=40 value=1
  print_list_using_fpn_RAM:
  ra[80000002]-
  ra[80000001]-
  ra[80000000]-
  ra[80000003]-

  print_pgtbl: 0 - 512
  00000000: 80000001
  00000004: 80000002
  Time slot  14
  Memory content [addr]-[offset,value]:
                 [80000000]-[20,100]
                 [80000001]-[40,1]
                 [80000002]-[64,102]| max size = 1024

          CPU 3: Put process  7 to run queue at time 15
          CPU 0: Processed  5 has finished at time 15
          CPU 0: Dispatched process  4 at time 15
  Time slot  15
          CPU 3: Dispatched process  7 at time 15
          CPU 2: Put process  6 to run queue at time 15
          CPU 2: Dispatched process  6 at time 15
          Loaded a process at input/proc/s1, PID: 8 PRIO: 0 at time 16
          CPU 1: Put process  2 to run queue at time 16
          CPU 1: Dispatched process  8 at time 16
  Time slot  16
          CPU 3: Put process  7 to run queue at time 17
          CPU 3: Dispatched process  7 at time 17
  Time slot  17
          CPU 2: Put process  6 to run queue at time 17
          CPU 2: Dispatched process  6 at time 17
          CPU 0: Put process  4 to run queue at time 17
          CPU 0: Dispatched process  2 at time 17
          CPU 1: Put process  8 to run queue at time 18
          CPU 1: Dispatched process  8 at time 18
  Time slot  18
          CPU 3: Put process  7 to run queue at time 19
  Time slot  19
          CPU 0: Put process  2 to run queue at time 19
          CPU 2: Put process  6 to run queue at time 19
          CPU 2: Dispatched process  6 at time 19
          CPU 0: Dispatched process  2 at time 19
          CPU 3: Dispatched process  7 at time 19
          CPU 2: Processed  6 has finished at time 20
  Time slot  20
          CPU 1: Put process  8 to run queue at time 20
          CPU 1: Dispatched process  8 at time 20
          CPU 2: Dispatched process  4 at time 20
          CPU 3: Put process  7 to run queue at time 21
          CPU 3: Dispatched process  7 at time 21
  Time slot  21
          CPU 0: Put process  2 to run queue at time 21
          CPU 0: Dispatched process  2 at time 21
          CPU 2: Put process  4 to run queue at time 22
  Time slot  22
          CPU 0: Processed  2 has finished at time 22
          CPU 0: Dispatched process  1 at time 22
  [PID: 1] read region=2 offset=20: INVALID REGION OFFSET
  print_list_using_fpn_RAM:
  ra[80000002]-
  ra[80000001]-
  ra[80000000]-
  ra[80000003]-

  print_pgtbl: 0 - 768
  00000000: 80000000
  00000004: 40000020
  00000008: 40000000
          CPU 1: Put process  8 to run queue at time 22
          CPU 1: Dispatched process  8 at time 22
  Memory content [addr]-[offset,value]:
                 [80000000]-[20,100]
                 [80000001]-[40,1]
                 [80000002]-[64,102]| max size = 1024

          CPU 2: Dispatched process  4 at time 22
          CPU 3: Put process  7 to run queue at time 23
          CPU 3: Dispatched process  7 at time 23
  Time slot  23
  [PID: 1] write region=3 offset=20 value=103
  print_list_using_fpn_RAM:
  ra[80000002]-
  ra[80000001]-
  ra[80000000]-
  ra[80000003]-
  [PID: 1] read region=3 offset=20: INVALID REGION OFFSET
  print_list_using_fpn_RAM:
  ra[80000002]-
  ra[80000001]-
  ra[80000000]-
  ra[80000003]-

  print_pgtbl: 0 - 768
  00000000: 80000000
  00000004: 40000020
  00000008: 40000000
  Memory content [addr]-[offset,value]:
                 [80000000]-[20,100]
                 [80000001]-[40,1]
                 [80000002]-[64,102]| max size = 1024

          CPU 3: Put process  7 to run queue at time 25
          CPU 3: Dispatched process  7 at time 25
  Time slot  25
          CPU 3: Processed  7 has finished at time 26
  Time slot  26
          CPU 2: Put process  4 to run queue at time 26
          CPU 0: Put process  1 to run queue at time 26
          CPU 0: Dispatched process  1 at time 26
  [PID: 1]----After enlist----
  print_list_rg:
  rg[100->600]

          CPU 3 stopped at time 26
          CPU 2: Dispatched process  4 at time 26
          CPU 2: Processed  4 has finished at time 27
          CPU 2 stopped at time 27
  Time slot  27
  Time slot  28
          CPU 0: Processed  1 has finished at time 28
          CPU 0 stopped at time 28
  ```

 

