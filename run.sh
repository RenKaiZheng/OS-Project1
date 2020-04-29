#!/bin/bash
for i in {1..5..1}
do 
	echo "FIFO_$i"
	sudo ./main < OS_PJ1_Test/FIFO_${i}.txt > output/FIFO_${i}_stdout.txt
	sudo dmesg | grep Project1 > output/FIFO_${i}_dmesg.txt
	sudo dmesg -c

	echo "SJF_$i"
	sudo ./main < OS_PJ1_Test/SJF_${i}.txt > output/SJF_${i}_stdout.txt
	sudo dmesg | grep Project1 > output/SJF_${i}_dmesg.txt
	sudo dmesg -c


	echo "PSJF_$i"
	sudo ./main < OS_PJ1_Test/PSJF_${i}.txt > output/PSJF_${i}_stdout.txt
	sudo dmesg | grep Project1 > output/PSJF_${i}_dmesg.txt
	sudo dmesg -c


	echo "RR_$i"
	sudo ./main < OS_PJ1_Test/RR_${i}.txt > output/RR_${i}_stdout.txt
	sudo dmesg | grep Project1 > output/RR_${i}_dmesg.txt
	sudo dmesg -c


done
