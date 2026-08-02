#!/bin/bash
echo "Build new version?"
read -p "[y\n]: " input
if [ "$input" = "y" ]; then
	echo "Building"
	gcc -o a.out main.c expr_eval.c
else
	echo "Aborted"
fi
echo "Done"
	