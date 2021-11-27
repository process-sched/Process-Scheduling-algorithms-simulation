
# Process Scheduling algorithms simulation

### Dependencies
* g++ compiler
	* `sudo apt install g++`
* python3-dev
	* `sudo apt install python3-dev`
* matplotlib
	* `pip3 install matplotlib`

### Input format
CSV file with each row having `PID,name,type,Priority,AT,BT`
<br>eg.
```
  0,P1,C,67,22,1
  1,P2,C,71,9,45
  2,P3,C,26,12,2
```

### Run instruction 
```
mkdir graph
g++ -I /usr/include/python3.8 mainCPP.cpp
./a.out <inputFileName.csv>
```
### Output
All the graphs will be saved inside the `/graph` folder 


