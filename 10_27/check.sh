#!bin/bash

#检查是否提供文件名参数
if [ $# -ne 1 ];then
    echo 
    exit 1
fi

filename="$1"