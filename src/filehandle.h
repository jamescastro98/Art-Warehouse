#include <stdio.h>
#include <stdlib.h>
#include "warehouse.h"
#include <string.h>
#include <ctype.h>
#include "linkedlist.h"


struct artList{
	char* rname;
	struct artList* next;
};

int adding=0; //0 if not adding, First ID if adding.
/*
//PROTOTYPES
struct linkedlist{
	struct warehouse_sf_list* headlist;
	struct linkedlist* next;
};

int addWarehouse(struct linkedlist* head, struct warehouse* newWare, int size , int priv);
int deleteArt(char* artName, struct linkedlist* sizelist);
int printOrdered(struct linkedlist* sizelist, int sizeorprice);
int splitWare(struct linkedlist* sizelist,struct warehouse* ware,int artsize);
void clearMem(struct linkedlist* sizelist);
int printLinked(struct linkedlist* sizelist, int pub);
int addArt(struct linkedlist* head, struct art_collection* art, int size);
int utilization(struct linkedlist* head);
int addNodeList(struct linkedlist* head, struct warehouse_sf_list* newNode);
*/
int loadWarehouse(char *fileName,struct linkedlist* link){
	FILE *file;
	char pub[]="public\n";
	char priv[]="private\n";
	if(file=fopen(fileName,"r")){
		char buffer[1000];
		while(fgets(buffer,1000,(FILE*) file)){
			int ispriv=0;
			if(strlen(buffer)<5){
				printf("Incorrect File Format\n");
				exit(0);
			}
			int currentID=atoi(buffer);
			if(currentID<0){
				goto incorrectformat;
			}
			int index=0;
			while(buffer[index]!=' '){
				if(index>1000){
					goto incorrectformat;
				}
				index++;
			}
			index++; //instead of space we have digit char
			int currentSize=atoi(buffer+index);
			//free(newbuff);

			while(buffer[index]!=' '){
				if(index>254){
					goto incorrectformat;
				}
				index++;
			}
			index++; //get past space
			char pubprivbuff[9];
			strcpy(pubprivbuff,buffer+index);

			if(strcmp(pubprivbuff,priv)==0){
				ispriv=1;
			}
			struct warehouse* lineWare = (struct warehouse*)(malloc(sizeof(struct warehouse)));
			lineWare->id=currentID;
			lineWare->size=currentSize;
			lineWare->art_collection=NULL;
			int success = addWarehouse(link,lineWare,currentSize,ispriv);
			if(success<0){
				printf("%s", "Error in File Format.");
				exit(0);
			}
		}
		printf("ADDED WAREHOUSE(S) FROM FILE.\n");
		return 0;
	}

incorrectformat:
	return -1;
}

int loadArt(char *fileName,struct linkedlist* head){
	int error=0;	
	struct artList* newHead=(struct artList*)(malloc(sizeof(struct artList)));
	newHead->rname=NULL;
	newHead->next=NULL;
	if(head==NULL){
		printf("No Warehouses Exist!");
		return 0;
	}
	FILE *file;
	if(file=fopen(fileName,"r")){
		char buffer[1000];
		while(fgets(buffer,1000,(FILE*) file)){
		if(strlen(buffer)<5){
	//	printf("Incorrect File Format\n");
		}
		int index=0;
		while(isdigit(buffer[index])==0){
			if(index>1000){
				return -1;
			}
			index++;
		}
		index--;
		char artName[index];
		char* name=(char*)malloc((index*sizeof(char)));
		memcpy(artName,buffer,index);
		artName[index]='\0';
		for(int j=0;artName[j];j++){
			artName[j]=tolower((unsigned char)artName[j]);
		}

		strcpy(name,artName);

		if(newHead->rname==NULL){
			newHead->rname=name;
		}

		else{
			struct artList* nod = newHead;
			while(nod->next!=NULL){
				nod=nod->next;
			}
			struct artList* newNod=(struct artList*)(malloc(sizeof(struct artList)));
			newNod->rname=name;
			nod->next=newNod;
		}
		index++;
		int size=atoi(buffer+index);
		if(size==0){
			error=1; 
		}
		while(buffer[index]!=' '){
			index++;
		}
		index++;
		int price=atoi(buffer+index);
		if(price==0){
			error=1;
		}
		struct art_collection* artcol=(struct art_collection*)malloc(sizeof(struct art_collection));
		artcol->name=name;
		artcol->size=size;
		artcol->price=price;

		int success = addArt(head,artcol,size);	
		if(error==1){
			filerr:
			printf("%s","Error in File Format.");
			
			while(newHead!=NULL){
				deleteArt(newHead->rname,head);
				newHead=newHead->next;
			}
			return 0;
			}
		}
	}
	printf("%s","ADDED ART COLLECTION(S) FROM FILE.\n");
	return 1;
}
