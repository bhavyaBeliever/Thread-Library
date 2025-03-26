#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
struct sumInput{
	int *x;
	int start;
	int end;
};
void* sum(void *args){
	struct sumInput *input = (struct sumInput *)args;  
	int ans = 0;
	for (int i = input->start; i < input->end; i++)
	{
		ans += input->x[i];
	}
	int * addr = (int*)malloc(sizeof(int));
	*addr = ans;
	return addr;
}
int main(){
	pthread_t t1, t2;
	
	struct sumInput input1, input2;
	int arr[] = {1,2,3,4};
	input1.x = arr;
	input1.start = 0;
	input1.end = 2;
	
	input2.x = arr;
	input2.start = 2;
	input2.end = 4;

	pthread_create(&t1, NULL, &sum, &input1);
	pthread_create(&t2, NULL, &sum, &input2);
	void *returnValue1, *returnValue2;
	pthread_join(t1, &returnValue1);
	pthread_join(t2, &returnValue2);
	
	int *ans = (int *)returnValue1;
	*ans += (int)(*((int *)returnValue2));
	printf("%d \n", *ans);
	return 0;
}
