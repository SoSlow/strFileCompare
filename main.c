#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
	char *str;
	size_t size;
	size_t reserved;
}DynamicString;

typedef struct 
{
	DynamicString **data;
	size_t size;
	size_t reserved;
}DynamicArray;

DynamicArray* CreateDynamicArray()
{
	DynamicArray *DA = (DynamicArray *)malloc(sizeof(DynamicArray));
	DA->size = 0;
	DA->reserved = 0;
	DA->data = NULL;
	return DA;
}

DynamicString* CreateDynamicString()
{
	DynamicString *DS = (DynamicString *)malloc(sizeof(DynamicString));
	DS->size = 0;
	DS->reserved = 0;
	DS->str = NULL;
	ResizeDS(DS, 1);

	return DS;
}

int DeleteLastDAEl(DynamicArray* DA)
{
	if(DA->size > 0)
	{
		free(DA->data[DA->size-1]);
		DA->size--;
	}
	return 0;
}

int DeleteLastDSEl(DynamicString* DS)
{
	if(DS->size > 0)
	{
		DS->size--;
	}
	return 0;
}

int ResizeDA(DynamicArray* DA, size_t new_size)
{
	while(DA->size > new_size)
	{
		DeleteLastDAEl(DA);
	}
	DA->reserved = 2 * new_size;
	DA->data =(DynamicString **)realloc(DA->data, sizeof(DynamicString *)*DA->reserved);
	DA->size=new_size;
	return 0;
}

int ResizeDS(DynamicString* DS, size_t new_size)
{
	while(DS->size > new_size)
	{
		DeleteLastDSEl(DS);
	}
	DS->reserved = 2 * new_size;
	DS->str =(char *)realloc(DS->str, sizeof(char)*DS->reserved);
	DS->size=new_size;
	DS->str[DS->size+1] = 0;
	return 0;
}

int PushBackDA(DynamicArray* DA, DynamicString *str)
{
	if(DA->size >= DA->reserved)
		ResizeDA(DA, DA->size+1);
	else
		DA->size++;
	DA->data[DA->size - 1] = str;
	return 0;
}

int PushBackDS(DynamicString *DS, char c)
{
	if(DS->size >= DS->reserved)
		ResizeDA(DS, DS->size+1);
	else
		DS->size++;
	DS->str[DS->size-2] = c;
	DS->str[DS->size-1] = 0;
	return 0;
}

int AssignDS(DynamicString *DS, char *str)
{
	int len = strlen(str);
	if(DS->reserved <= len)
		ResizeDS(DS, len+1);
	else
		DS->size = len +1;
	strcpy(DS->str, str);
	return 0;
}

DynamicString* FGetDS(FILE *f)
{
	DynamicString* DS = CreateDynamicString();
	char c = 1;
	while(!feof(f) && c!='\n')
	{
		c = fgetc(f);
		if(c == EOF || c == 0xD || c == 0xA)
			continue;
		PushBackDS(DS, c);
	}
	if(DS->size != 1)
		return DS;
	else
		//add ordelete DS
		return NULL;
}

int main(void)
{
	FILE *f1;
	FILE *f2;
	DynamicString *DS;
	DynamicArray *DA1 = CreateDynamicArray();
	DynamicArray *DA2 = CreateDynamicArray();
	size_t i, j, flag;
	f1 = fopen("1.txt", "r");
	f2 = fopen("2.txt", "r");

	while(DS = FGetDS(f1))	
		PushBackDA(DA1, DS);	

	while(DS = FGetDS(f2))	
		PushBackDA(DA2, DS);	

	for(i = 0; i < DA1->size; ++i)
		printf("\"%s\"\n", DA1->data[i]->str);

	printf("\nFile 2\n");
	for(i = 0; i < DA2->size; ++i)
		printf("\"%s\"\n", DA2->data[i]->str);

	for(i = 0; i < DA1->size; ++i)	
	{
		flag = 0;
		for(j = 0; j < DA2->size; ++j)		
			if(strcmp(DA1->data[i]->str, DA2->data[j]->str) == 0)
			{
				flag = 1;
				break;
			}
		if(!flag)
		{	
			break;
		}
	}
	if(flag)
		printf("files are equal for us\n");
	else
		printf("files are different for us\n");

	fclose(f1);
	fclose(f2);
	return 0;
}