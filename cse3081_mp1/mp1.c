#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void print_result(char* file_name, int index, int rows, int cols, int max, float time );
int main(int argc, char *argv[])
{	
	//arg
	int flag = atoi(argv[2]);
	//printf("argv[2] is %s\n", argv[2]);
	
	int cols=0;
	int rows=0;
	int i,j = 0;
	float time =0;
	FILE *fp;
		
	clock_t start, end;
	start = clock();
	fp = fopen(argv[1],"r");
	
	if(fp != NULL){
		fscanf(fp,"%d %d", &rows,&cols);
		int arr[rows+1][cols+1];
		for(i=0; i<rows;i++){
			for (j =0 ; j <cols; j++){
				fscanf(fp, "%d", &arr[i][j]);
			}
		}
		fclose(fp);
		int a=0;
		int b =0;
		int c =0 ;
		int d= 0;
		int e = 0;
		int f =0 ;
		int max_sum = INT_MIN;
		// O(n^6)
		if (flag==1){
			for(a=0; a<rows; a++){
				for(b=0;b<=a; b++){
					for(c =0 ; c<cols; c++){
						for(d=0; d<=c; d++){
							int temp_sum = 0;
							for(e=b; e<=a; e++){
								for(f=d; f<=c; f++){
									temp_sum+=arr[e][f];
								}
							}
							if (temp_sum>max_sum){
								max_sum = temp_sum;
							}
							
						}
					}
				}
			}
			//printf("max_sum %d ", max_sum);
			end = clock();
			time = (end-start);
			print_result(argv[1], flag, rows, cols, max_sum, time );
		
			//printf("%.3lf√ ",time);
			//print_result(fp_out,flag,rows,cols,max_sum,time);
		}
		//O(n^4)
		if (flag ==2){
			//printf("2nd is started");
			int max_sum = INT_MIN;
			for(a=0 ; a<rows; a++){
				int temp_max_sum_2 = 0;
				for(b=0; b<=a; b++){
					int temp_sum=0;
					int temp_max = 0;
					for(c=0;c<cols;c++){
						for(d=b; d<=a; d++){
							temp_sum += arr[d][c];
							//printf("temp %d \n",temp_sum);
						}
						if(temp_sum<0){
							temp_sum=0;
						}
						if(temp_sum>temp_max){
							temp_max= temp_sum;
						}
					}
					//printf("a=%d, b =%d temp_sum is %d\n", a,b,temp_max);
					if(temp_max>temp_max_sum_2){
						temp_max_sum_2 = temp_max;
					}
				}
				if (temp_max_sum_2>max_sum){
					max_sum = temp_max_sum_2;	
				}
						
			}
			//printf("max sum is %d\n",max_sum);
			end = clock();
			time = (end-start);
			print_result(argv[1], flag, rows, cols, max_sum, time );
		}
		if (flag==3){
			int max_sum =INT_MIN;
			for (a=0; a<rows;a++){
				int temp_arr[cols];
				
				for (f = 0; f<cols; f++){
					temp_arr[f]=0;
				}
				for (b=a; b<rows; b++){
	
					int temp_sum = 0;
					int temp_max=INT_MIN;
					for (c=0 ; c<cols;c++){
						temp_arr[c]+=arr[b][c];
					}
					for (d=0 ; d<cols;d++){
						
						temp_sum+=temp_arr[d];
						if(temp_sum<0){
							temp_sum=0;
						}
						else if(temp_sum>temp_max){
							temp_max = temp_sum;
						}
						//printf("d = %d, temp = %d, temp max = %d, max sum = %d\n ",d,temp_arr[d], temp_max, max_sum);
					}
					if(temp_max>max_sum){
						max_sum= temp_max;
					}
				}
			}
			//printf("max sum is %d\n",max_sum);
			end = clock();
			time = (end-start);
			print_result(argv[1], flag, rows, cols, max_sum, time );
		}
	
	}
	else{
		printf("File Error");
	}
	// O(n^6)	 
}

void print_result(char* file_name, int index, int rows, int cols, int max, float time ){
	FILE *fp_out;

	char final_file_name[255]= "result_";
	strcat(final_file_name,file_name);
	fp_out = fopen(final_file_name,"w");

	fprintf(fp_out, "%s\n", file_name);
	fprintf(fp_out, "%d\n", index);
	fprintf(fp_out, "%d\n", rows);
	fprintf(fp_out, "%d\n", cols);
	fprintf(fp_out, "%d\n", max);
	fprintf(fp_out, "%0.1lf\n", time);


	fclose(fp_out);
	
}

