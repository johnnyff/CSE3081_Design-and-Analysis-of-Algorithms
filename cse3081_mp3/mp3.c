#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>



struct treenode{
	int	freq;
	unsigned int ch;
	struct treenode* left_node;
	struct treenode* right_node;
	struct treenode* next;
};
typedef struct treenode treenode;

struct linkednode
{
	int freq;
	unsigned int ch;
	struct linkednode* next;
};
typedef struct linkednode linkednode;


void print_result(char* file_name, int index, int rows, int cols, int max, float time);
void insert_linkedlist(struct treenode* value);
int isLeaf(struct treenode* root);
void make_code_table(struct treenode* root, int arr[],int top,int code_table[]);
int printArr(int arr[], int n);
void print_tree_node(struct treenode* root);
struct treenode* make_huffman_tree();
struct treenode* newtreenode(char character, unsigned freq);
struct treenode* get_first_one();
void invert_table(int codeTable1[],int codeTable2[]);


struct treenode* head =NULL;

int main(int argc, char* argv[])
{
	unsigned int freq[256] = { 0 };

	int ch = 0;
	int i, j = 0;
	float time = 0;
	int total_n = 0;
	char command[50] ;
	strcpy(command, argv[1]);
	char command_1[10] = "-c";
	char command_2[10] = "-d";
	FILE* fp;
	FILE* fp_out;
	char read_file_name[50];
	char file_name[50] ;
	clock_t start, end;
	start = clock();
	
	if(!strcmp(command,command_1)){
		strcpy(read_file_name, argv[2]);
		
	}
	else if(!strcmp(command,command_2)){
		
		char* new_name =NULL;
		new_name = strtok(argv[2], ".");
		strcpy(read_file_name, new_name);
		strcat(read_file_name, ".txt");
	}
	else{
		printf("\ncommand is not valid\n");
		return;
	}

	fp = fopen(read_file_name, "r");
	if (fp != NULL) {
		printf("\nstarted ");
		while (1){
			ch = fgetc(fp);
			if (ch == EOF) {
				break;
			}
			putchar(ch);
			freq[ch] += 1;
		} 
		fclose(fp);
		
		for (i = 0; i < 256; i++) {
			if (freq[i] != 0) {
				total_n += 1;
				if (total_n == 1) {
					head = (struct treenode*)malloc(sizeof(struct treenode));
					head->freq = freq[i];
					head->ch = i;
					head->next = NULL;
					head->left_node =NULL;
					head->right_node= NULL;
				}
				else {

					struct treenode* newNode = (struct treenode*)malloc(sizeof(struct treenode));
					newNode->freq = freq[i];
					newNode->ch = i;
					newNode->next = NULL;
					newNode->left_node =NULL;
					newNode->right_node =NULL;
					struct treenode* current = head;
					struct treenode* prev = head;
					if (current->freq > newNode->freq) {
						newNode->next = current;
						head = newNode;
					}
					else {
						while (current->next != NULL) {
							if (current->freq > newNode->freq) {
								break;
							}
							else {
								prev = current;
								current = current->next;
							}
						}
						newNode->next = prev->next;
						prev->next = newNode;
					}
					
				}
			}
		}
		
 		
		struct treenode* root = make_huffman_tree();
		
		int codeTable[256]={0};
		int inverted_codeTable[256]={0};


		struct treenode* tree_map = root;
		//print_tree_node(root);

		int arr[255], start = 0;
    	make_code_table(root, arr, start, codeTable);
		invert_table(codeTable, inverted_codeTable);

		if(!strcmp(command,command_2)){
			FILE* fp_out2;
			strcat(read_file_name, ".zz");
			fp = fopen(read_file_name, "r");
			strcat(read_file_name, ".yy");
			fp_out2 = fopen(read_file_name,"w");
			decompressFile (fp, fp_out2, tree_map);
			fclose(fp);
			fclose(fp_out2);
			
			}
		else if(!strcmp(command,command_1)){
			strcpy(file_name, read_file_name);
			strcat(file_name, ".zz");
			fp = fopen(read_file_name, "r");
			fp_out = fopen(file_name,"w");

			compressFile(fp,fp_out,inverted_codeTable);
			fclose(fp);
			fclose(fp_out);
		}
	    
		
		
		//printf("done");
	}

	else {
		printf("\nFile Error\n");
	}
	// O(n^6)	 
}
void compressFile(FILE *input, FILE *output, int codeTable[]){
    char bit, c, x = 0;
    int n,length,bitsLeft = 8;
	if(input==NULL){
		printf("inputfile error\n");
		return;
	}
    while (1)
	{
		c = fgetc(input);
		if (c == EOF) {
				break;
			}
		length = (int)log10(codeTable[c])+1;
		n=codeTable[c];
        while (length>0){
            bit = n % 10 - 1;
            n /= 10;
            x = x | bit;
            bitsLeft--;
            length--;
            if (bitsLeft==0){
                fputc(x,output);
                x = 0;
                bitsLeft = 8;
            }
            x = x << 1;
        }
    }
    if (bitsLeft!=8){
        x = x << (bitsLeft-1);
        fputc(x,output);
    }


    return;
}
void decompressFile (FILE *input, FILE *output, treenode* root){
    treenode *current = root;
    char c,bit;
    char template = 1 << 7;
    int i;
    while ((c=fgetc(input))!=EOF){
       // printf("c is %c \n",c);
		for (i=0;i<8;i++){
            bit = c & template;
            c = c << 1;
            if (bit==0){
                current = current->left_node;
				if (current->ch !=-128){
					fputc(current->ch, output);
					//printf("current->ch: %c \n",current->ch);
					current = root;
				}
            }
            else{
                current = current->right_node;
				//printf("current : %c \n",current->ch);

                if (current->ch !=-128){
					fputc(current->ch, output);
					//printf("current->ch: %c \n",current->ch);
					current = root;
				}
                
            }
        }
        
    }

    return;
}
void print_tree_node(struct treenode* root){
	if(root!=NULL){
		print_tree_node(root->left_node);
		printf("%c(%d): %d \n",root->ch,root->ch, root->freq);
		print_tree_node(root->right_node);
	}
}
struct treenode* make_huffman_tree(){
	struct treenode* left ;
	struct treenode* right ;
	struct treenode* top ;
	struct treenode* res = head;
	int flag =0;
	//printf("start!");
	int cnt = 0;
	while (head->next!=NULL) {
		left = get_first_one();
		right = get_first_one();
		
		if(head ==NULL){
			top = newtreenode(128,left->freq+right->freq);
			top->left_node = left;
			top->right_node = right;
			break;
		}

		top = newtreenode(128,left->freq+right->freq);
		
		top->left_node = left;
		top->right_node = right;
		
		insert_linkedlist(top);
		//printf("Head is %d top is \n", head->freq,top->freq);
		
	}
	//printf("end!");

	return top;
}
void invert_table(int codeTable1[],int codeTable2[]){
    int i, temp, k;

    for (i=0;i<256;i++){
        k = codeTable1[i];
        temp = 0;
        while (k>0){
            temp = temp * 10 + k %10;
            k /= 10;
        }
        codeTable2[i]=temp;
    }
	return;
}
struct treenode* get_first_one(){
	struct treenode* temp = (struct treenode*)malloc(sizeof(struct treenode));
	temp = head;
	head = head->next;
	return temp;
}
struct treenode* newtreenode(char character, unsigned freq){
	struct treenode* new = (struct treenode*)malloc(sizeof(struct treenode));
	new->left_node = new->right_node = NULL;
	new->ch = character;
	new->freq = freq;
	new->next =NULL;
	return new;
}
int isLeaf(struct treenode* root)
{
    return !(root->left_node) && !(root->right_node);
}
void make_code_table(struct treenode* root, int arr[],int top,int code_table[])
{
    if (root->left_node) {
		//printf("left : %d\n", root->left_node->freq);
        arr[top] = 1;
        make_code_table(root->left_node, arr, top + 1, code_table);
    }
 
    if (root->right_node) {
 		//printf("right : %d\n", root->right_node->freq);
        arr[top] = 2;
        make_code_table(root->right_node, arr, top + 1,code_table);
    }
 
    if (isLeaf(root)) {
		int t  = printArr(arr, top);

		code_table[(int)root->ch]= t;
        //printf("%c ->before : %d , after: %d\n : ",root->ch, (int)root->ch, t);
    }
}
int printArr(int arr[], int n)
{
    int i;
	int t =0;
	//printf("check : ");
    for (i = 0; i < n; ++i)
		t= t*10+arr[i];
        //printf("%d ",arr[i]);
	//printf("      %d", t);
    //printf("\n");

	return t;
}
void insert_linkedlist(struct treenode* newnode) {
	struct treenode* current = head;
	while(current->next!= NULL && newnode->freq>current->freq){
		current= current->next;
	}
	newnode->next = current->next;
	current->next = newnode;
}
/*
void print_result(char* file_name, int index, int rows, int cols, int max, float time) {
	FILE* fp_out;

	char final_file_name[255] = "result_";
	strcat(final_file_name, file_name);
	fp_out = fopen(final_file_name, "w");

	fprintf(fp_out, "%s\n", file_name);
	fprintf(fp_out, "%d\n", index);
	fprintf(fp_out, "%d\n", rows);
	fprintf(fp_out, "%d\n", cols);
	fprintf(fp_out, "%d\n", max);
	fprintf(fp_out, "%0.1lf\n", time);


	fclose(fp_out);

*/
void Swap(int arr[], int a, int b)
{
	int temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

