#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX_SIZE 100000000
void print_result(char* file_name, int index, int length, float time,int arr[]);
void Swap(int arr[], int a, int b) ;
int Partition(int arr[], int left, int right);
void quicksort(int arr[], int left, int right);
void mergesort(int arr[], int left, int right);
void merge(int arr[], int left, int right);

void three_try_quicksort(int arr[], int left, int right);
int three_try_partition(int arr[], int left, int right);

int sorted_array[MAX_SIZE];

int main(int argc, char *argv[])
{	
	//arg
	int flag = atoi(argv[2]);
	//printf("argv[2] is %s\n", argv[2]);
	
	int length=0;
	int i,j = 0;
	float time =0;
	FILE *fp;
		
	clock_t start, end;
	
	fp = fopen(argv[1],"r");
	
	if(fp != NULL){
		fscanf(fp,"%d", &length);
		int* arr= (int*)malloc(sizeof(int)*length) ;
		for(i=0; i<length;i++){
			fscanf(fp, "%d", &arr[i]);
		}
		fclose(fp);
		start = clock();
		// Insertion Sort
		if (flag==1){
			int key,i,j;
			for (i = 1; i < length; i++) {
				key = arr[i];
				for (j = i - 1; j >= 0&& arr[j]>key;j--) {
					arr[j + 1] = arr[j];
				}
				arr[j + 1] = key;
			}
			end = clock();
			time = (end - start);
			print_result(argv[1], flag, length, time, arr);
		}
		//Quick Sort
		if (flag ==2){
			quicksort(arr,0,length-1);
			end = clock();
			time = (end - start);
			print_result(argv[1], flag, length, time, arr);
		}
		if (flag==3){
			mergesort(arr,0,length-1);
			end = clock();
			time = (end - start);
			print_result(argv[1], flag, length, time, arr);
		}
		if(flag==4){
			three_try_quicksort(arr,0,length-1);
			end = clock();
			time = (end - start);
			print_result(argv[1], flag, length, time, arr);
		}
		
	}
	else{
		printf("File Error");
	}
}

void print_result(char* file_name, int index, int length, float time,int arr[]){
	FILE *fp_out;
	int i;
	char t1[10];
	char final_file_name[255]= "result_";
	sprintf(t1, "%d_",index);
	strcat(final_file_name,t1);
	strcat(final_file_name,file_name);

	fp_out = fopen(final_file_name,"w");

	fprintf(fp_out, "%s\n", file_name);
	fprintf(fp_out, "%d\n", index);
	fprintf(fp_out, "%d\n", length);
	fprintf(fp_out, "%0.6lf\n", time/1000);
	for(i=0; i<length; i++){
		fprintf(fp_out, "%d ", arr[i]);
	}

	fclose(fp_out);
	
}
void Swap(int arr[], int a, int b) 
{
	int temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}
int Partition(int arr[], int left, int right)
{
	int pivot = arr[right]; 
	int i = left;
	int j;
	for (j=left; j<right;j++){
		if(arr[j]<=pivot){
			Swap(arr,i,j);
			i+=1;
		}
	}
	Swap(arr, i, right); 
	return i;  
}
void quicksort(int arr[], int left, int right){
	int q;
	if(right>left){
		q= Partition(arr, left, right);
		quicksort(arr,left, q-1);
		quicksort(arr,q+1,right);
	}
	else return;
}

void mergesort(int arr[], int left, int right){
	if(left<right){
		int mid = (left+right)/2;
		mergesort(arr, left, mid);
		mergesort(arr,mid+1, right);
		merge(arr,left,right);
	}
}
void merge(int arr[], int left, int right){
	int i;
	int mid = (left+right)/2;
	int a = left;
	int b = mid+1;
	int c = left;
	while(a<=mid && b<=right){
		if(arr[a]<arr[b]){
			sorted_array[c] = arr[a];
			c+=1;
			a+=1;
		}
		else{
			sorted_array[c] = arr[b];
			c+=1;
			b+=1;
		}
	}

	if(a>mid){
		while(b<=right){
			sorted_array[c] = arr[b];
			c+=1;
			b+=1;
		}
	}
	else{
		while(a<=mid){
			sorted_array[c] = arr[a];
			c+=1;
			a+=1;
		}
	}
	for(i=left; i<=right;i++){
		arr[i] = sorted_array[i];
	}
}

void three_try_quicksort(int arr[], int left, int right){
	int q;
	
	if(right>left){
		if((right-left)>50){
			if(arr[left]>arr[(left+right)/2]){
			Swap(arr,left,(left+right/2));
			}
			if(arr[(left+right)/2]>arr[right]){
				Swap(arr,(left+right)/2, right);
			}
			if(arr[left]>arr[right]){
				Swap(arr,right,left);
			}
			Swap(arr, right,(right-left)/2);
			q= Partition(arr, left, right);
			quicksort(arr,left, q-1);
			quicksort(arr,q+1,right);
		}
		else{
			int key,i,j;
			for (i = left; i <= right; i++) {
				key = arr[i];
				for (j = i - 1; j >= 0&& arr[j]>key;j--) {
					arr[j + 1] = arr[j];
				}
				arr[j + 1] = key;
			}
		}
	}
}
int three_try_partition(int arr[], int left, int right){
	int i,j,k;
	int partition= right;
	int distance= right-left;
	for(i=0; i<3; i++){
		int cnt=0;
		k = rand()%(right-left+1)+left;
		for(j=left; j<=right;j++){
			if(arr[k]<arr[j]){
				cnt+=1;
			}		
		}
		if(abs(right-left-cnt)<distance){
			partition = k;
			distance = abs(right-left-cnt);
		}
	}
	int pivot = arr[partition]; 
	i = left;
	for (j=left; j<right;j++){
		if(arr[j]<=pivot){
			Swap(arr,i,j);
			i+=1;
		}
	}
	Swap(arr, i, right); 
	return i;  
}
