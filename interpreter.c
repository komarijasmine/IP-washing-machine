#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h" 

	
int interpretLine(char *str)
{	
	//delimiter to split
	char* separater = strtok(str, " ");

	//store token
	char* token;

	//first token
	token = strtok(str, separater);
	char* opName = token;
	int count =0; 
	char* token2;
	char* token3;
	int num; 
	char *endptr;

	//continue
	while(separater != NULL){

		//pass to next token
		separater = strtok(NULL, " ");

		count++;
		if (count == 1)
			token2 = token;
		else if (count == 2)
			token3  = token;
		else
			return 1;

	} 
	
	if (!strcmp(opName, "Ass")){
		num = strtol(token3, &endptr, 10);
		if (count != 2)
		{
			return 1;
		}
		if (assign(token2,num))
		{
			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}

	if (!strcmp(opName, "Inc"))
	{

		num = strtol(token3, &endptr, 10);
		if (count !=2)
		{
			return 1;
		}
		if (increase(token2,num)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}
	
	if (!strcmp(opName, "Dec"))
	{

		num = strtol(token3, &endptr, 10);
		if (count !=2)
		{
			return 1;
		}
		if (decrease(token2,num)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}
				
	if (!strcmp(opName,"Mal"))
	{
	
		num = strtol(token3, &endptr, 10);
		if (count !=2)
		{
			return 1;
		}
		if (allocate(token2,num)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}
			
	if (!strcmp(opName,"Pri"))
	{
	
		num = strtol(token3, &endptr, 10);
		if (count !=2)
		{
			return 1;
		}
		if (printCell(token2,num)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}
		

	if(!strcmp(opName,"Add"))
	{
		if (count !=2)
		{
			return 1;
		}
		if (add(token2,token3)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}
		
	if(!strcmp(opName,"Sub"))
	{
		if (count !=2)
		{
			return 1;
		}
		if (subtract(token2,token3)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}
		
	if(!strcmp(opName,"Mul"))
	{
		if (count !=2)
		{
			return 1;
		}
		if (multiply(token2,token3)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}
	
	if(!strcmp(opName,"And"))
	{
		if (count !=2)
		{
			return 1;
		}
		if (andCells(token2,token3)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}
		
	if(!strcmp(opName, "Xor"))
	{
		if (count !=2)
		{
			return 1;
		}
		if (xorCells(token2,token3)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}

	if(!strcmp(opName, "Fre"))
	{
		if (count !=1)
		{
			return 1;
		}
		if (freeArray(token2)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}


	
	if(!strcmp(opName, "Pra"))
	{
		if (count !=1)
		{
			return 1;
		}
		if (printArray(token2)){

			fprintf(stderr, "Assign value %s to the array with identifier %s failed", token3, token2);
			return 1;
		}
	}

	return 1;
}
