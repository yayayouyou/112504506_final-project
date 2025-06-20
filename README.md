

# 以下是各題操作指令 (112504506 資電二 石亞右)



## 📌 Q3 — Config last level cache to 2-way and full-way associative, test quicksort



### 2-way associative		

```sh
./build/X86/gem5.opt configs/example/se.py \
  -c ~/benchmark/quicksort \
  --cpu-type=TimingSimpleCPU \
  --caches --l2cache --l3cache \
  --l3_assoc=2 \
  --l1i_size=2kB --l1d_size=2kB \
  --l2_size=8kB --l3_size=16kB \
  --mem-type=NVMainMemory \
  --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config \
  > quick_2way_output.log
```
### | full-way associative (模擬: 1 set) |

```sh
./build/X86/gem5.opt configs/example/se.py \
  -c ~/benchmark/quicksort \
  --cpu-type=TimingSimpleCPU \
  --caches --l2cache --l3cache \
  --l3_assoc=1 \
  --l1i_size=2kB --l1d_size=2kB \
  --l2_size=8kB --l3_size=16kB \
  --mem-type=NVMainMemory \
  --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config \
  > quick_full_output.log
```
## 📌 Q4 — Modify last level cache replacement policy

### Baseline LRU		
```sh

./build/X86/gem5.opt configs/example/se.py \
  -c ~/benchmark/quicksort \
  --cpu-type=TimingSimpleCPU \
  --caches --l2cache --l3cache \
  --l3_assoc=2 \
  --l1i_size=2kB --l1d_size=2kB \
  --l2_size=8kB --l3_size=16kB \
  --l3_replacement_policy=LRU \
  --mem-type=NVMainMemory \
  --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config \
  > quick_LRU_output.log

```
### | Frequency-Based (LFU) |

```sh
./build/X86/gem5.opt configs/example/se.py \
  -c ~/benchmark/quicksort \
  --cpu-type=TimingSimpleCPU \
  --caches --l2cache --l3cache \
  --l3_assoc=2 \
  --l1i_size=2kB --l1d_size=2kB \
  --l2_size=8kB --l3_size=16kB \
  --l3_replacement_policy=LFU \
  --mem-type=NVMainMemory \
  --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config \
  > quick_LFU_output.log
```
## 📌 Q5 — Test the performance of write back and write through policy based on 4-way associative cache with isscc_pcm

### WB

```sh
./build/X86/gem5.opt  --debug-flags=Cache configs/example/se.py -c ~/benchmark/multiply  --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l3_assoc=4 --l1i_size=16kB --l1d_size=16kB --l2_size=256kB --l3_size=1MB --l3_replacement_policy=LRU --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > multiply_WB_output.log

```
### WT(要先改內容再執行)
```sh
./build/X86/gem5.opt  --debug-flags=Cache configs/example/se.py -c ~/benchmark/multiply  --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l3_assoc=4 --l1i_size=16kB --l1d_size=16kB --l2_size=256kB --l3_size=1MB --l3_replacement_policy=LRU --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > multiply_WT_output.log
```


## 📌 bouns — Design last level cache policy to reduce the energy consumption of pcm_based main memory

### baseline LRU (hello.o)
```sh
 ./build/X86/gem5.opt configs/example/se.py -c tests/test-progs/hello/bin/x86/linux/hello --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l1i_size=1kB --l1d_size=1kB --l2_size=2kB --l3_size=4kB --l3_replacement_policy=LRU --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > base_LRU_output.log
 ```

### modified WALRU (hello.o) 這個WALRU 跟LRU沒關係
```sh
 ./build/X86/gem5.opt configs/example/se.py -c tests/test-progs/hello/bin/x86/linux/hello --cpu-type=TimingSimpleCPU --caches --l2cache --l3cache --l1i_size=1kB --l1d_size=1kB --l2_size=2kB --l3_size=4kB --l3_replacement_policy=WALRU --mem-type=NVMainMemory --nvmain-config=../NVmain/Config/PCM_ISSCC_2012_4GB.config > base_WALRU_output.log
```

