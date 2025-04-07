#
# Makefiel for Uniform to Gaussian Distribution                  
# Chris K Cockrum                                   
# https://cockrum.net                               
# 02/03/2018                                        

default: ugdist.c
	gcc -o ugdist ugdist.c -s -O6 -lm


