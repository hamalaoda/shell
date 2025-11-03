#!/bin/bash

num1=2
num2=5
num3=8

max=$num1

if [ $num2 -gt $max ]; then
	max=$num2
fi
if [ $num3 -gt $max ]; then
	max=$num3
fi

echo "$max"
