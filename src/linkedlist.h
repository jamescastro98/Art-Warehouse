#include "warehouse.h"
#include <stdlib.h>
#include <stdio.h>
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
struct linkedlist{
	struct warehouse_sf_list* headlist;
	struct linkedlist* next;
};

int idcount = 5000;
//pub=0 - all, pub=1 - private, pub=2-public

int addWarehouse(struct linkedlist* head, struct warehouse* newWare, int size, int priv);

int deleteArt(char* artName,struct linkedlist* sizelist){
	int found=0;
	int ispublic=0; // 0 if pub, 1 if priv
	uint64_t zero=(uint64_t) 0;
	uint64_t one= (uint64_t) 1;
	uint64_t two=(uint64_t) 2;
	while(sizelist!=NULL){
	struct warehouse_sf_list* currentWaresf=sizelist->headlist;
	while(currentWaresf!=NULL){
	struct warehouse_list* warehouselist=(currentWaresf->warehouse_list_head);
		while(warehouselist!=NULL){
	
		if(((warehouselist->meta_info)&two)>zero){
		struct warehouse* ware=(warehouselist->warehouse);
		struct art_collection* col=(ware->art_collection);
		if(strcmp((col->name),artName)==0){	
		if((warehouselist->meta_info&one)>0){
		ispublic=1;
		}
		found=1;
		free(col->name);
		free(col);
		ware->art_collection=NULL;
		int redobit = ware->size;
		uint64_t newbit=(uint64_t) redobit;
		redobit=redobit<<2;
		uint64_t priv= (uint64_t) ispublic;
		redobit=redobit|priv;
		warehouselist->meta_info=redobit;
		}
		}
		
		warehouselist=warehouselist->next_warehouse;
	}
	currentWaresf=currentWaresf->sf_next_warehouse;
	 }
	sizelist=sizelist->next;
	}
	return found;
}

int printOrdered(struct linkedlist* sizelist,int sizeorprice){
	int numfound=1;
	uint64_t zero=(uint64_t) 0;
	uint64_t one=(uint64_t) 1;
	uint64_t two=(uint64_t) 2;
	int max;
//	struct linkedlist* currNode=sizelist;
//	struct art_collection* col=NULL;
	

	//if(sizeorprice==1){
//	char* an= (char*) malloc(255);
	
	while(numfound!=0){
	max=0;
	struct linkedlist* currNode=sizelist;
	numfound=0;
	struct art_collection* col=NULL;
	while(currNode!=NULL){
	struct warehouse_sf_list* sfl = currNode->headlist;
	while(sfl!=NULL){
	struct warehouse_list* wl = sfl->warehouse_list_head;

	while(wl!=NULL){
	if(((wl->meta_info)&two)>0){
	//we know it has art in it
	numfound++;
	struct art_collection* occup=wl->warehouse->art_collection;
	if(sizeorprice==0){
		if((occup->size)>max){
		max=occup->size;
		col=occup;
	//strcpy(an,occup->name);
		}
	}
	else{
		if(occup->price>max){
		max=occup->price;
		col=occup;
		}
	}
	}
	wl=wl->next_warehouse;
	}
	sfl=sfl->sf_next_warehouse;
	}
	currNode=currNode->next;
	}
	if(numfound>0){
	printf("%s",col->name);
	printf("%s"," ");
	printf("%d",col->size);
	printf("%s"," ");
	printf("%d",col->price);
	printf("%s","\n");
	deleteArt(col->name,sizelist);
	}
	
	}
       
	//end loop
	return 1;
}

void printascii(struct linkedlist* sizelist){
	struct linkedlist* currNode=sizelist;

	while(currNode!=NULL){
	
	struct warehouse_sf_list* curr=sizelist->headlist;
	while(curr!=NULL){
	struct warehouse_list* currList=(curr->warehouse_list_head);
	while(currList!=NULL){
	printf("_________________\n");
	printf("\n");
	printf("|_____     _____|\n");
	if(currList->warehouse==NULL){
	printf("|ART WARE  |OPEN|\n");
	}
	else{
	printf("|ART WARE |TAKEN|\n");
	}
	printf("|----  ___  ----|\n");
	printf("|     |   |     |\n");
	printf("|     |   |     |\n");
	printf("|_______________|\n");
	currList=currList->next_warehouse;
	}
	curr=curr->sf_next_warehouse;
	}
	currNode=currNode->next;
	}
}


int splitWare(struct linkedlist* sizelist,struct warehouse* ware,int artsize){
	struct linkedlist* currNode=sizelist;
	while(currNode!=NULL){
	//find current warehouse in list
	struct warehouse_sf_list* wsf=currNode->headlist;
	while(wsf!=NULL){
	if((wsf->class_size)==ware->size){
	struct warehouse_list* wl=wsf->warehouse_list_head;
	if(wl->warehouse==ware){
	//CASE IF IT IS THE HEAD
	wsf->warehouse_list_head=wl->next_warehouse;
	}
	else{
	while(wl!=NULL){
	struct warehouse_list* oldwl = wl;
	wl=wl->next_warehouse;
	if(wl->warehouse==ware){
	oldwl->next_warehouse=wl->next_warehouse;
	}}}
	//At this point, the warehouse is removed from the list.
	int newWareSize=(ware->size)-artsize;
	ware->size=artsize;
	int wareone = addWarehouse(sizelist,ware,artsize,1); //HERE
	struct warehouse* splitWare=(struct warehouse*)malloc(sizeof(struct warehouse));
	splitWare->id=idcount;
	idcount++;
	splitWare->size=newWareSize;
	splitWare->art_collection=NULL;
	int waretwo = addWarehouse(sizelist,splitWare,newWareSize,1);
	return 1;
	}
	wsf=wsf->sf_next_warehouse;
	}
	currNode=currNode->next;
	}
	return -1;
}
void clearMem(struct linkedlist* sizelist){
	uint64_t zero=(uint64_t) 0;
	uint64_t one=(uint64_t) 1;
	uint64_t two=(uint64_t) 2;

	while(sizelist!=NULL){
	struct warehouse_sf_list* currentWaresf=sizelist->headlist;
	while(currentWaresf!=NULL){
	struct warehouse_list*  wl = (currentWaresf->warehouse_list_head);
		
	currentWaresf=currentWaresf->sf_next_warehouse;
	}
	sizelist=sizelist->next;
	}
}
int printLinked(struct linkedlist* sizelist, int pub){
	uint64_t zero= (uint64_t) 0;
	uint64_t one=(uint64_t) 1;
	uint64_t two=(uint64_t)2;
	int isIn=0;
	while(sizelist!=NULL){
	struct warehouse_sf_list* currentWaresf = sizelist->headlist;

		while(currentWaresf!=NULL){
		struct warehouse_list* warehouselist=(currentWaresf->warehouse_list_head);
			
			while (warehouselist!=NULL){

			if(((warehouselist->meta_info)&two) > zero){
			//at this point we know our node is occupied.
			isIn=1;
			struct warehouse* ware=(warehouselist->warehouse);
			struct art_collection* col=(ware->art_collection);
			if(pub==4){
			printf("-----------\n");
			printf("|         |\n");
			printf("|   |OCCUP|\n");
			printf("|   ___   |\n");
			printf("|+++| |+++|\n");
			printf("\n");
			}
			if(((warehouselist->meta_info&one)==0) && (pub<2)){
			//node is public and we print for pub!=2
			//struct warehouse* ware=(warehouselist->warehouse);
			//struct art_collection* col=(ware->art_collection);
			printf("%s",(col->name));
			printf("%s"," ");
			printf("%d",(col->size));
			printf("%s", " ");
			printf("%d",(col->price));
			printf("%s","\n");	
			}
			
			if(((warehouselist->meta_info&one)==1) && ((pub==0 || pub==2))){
			printf("%s",(col->name));
			printf("%s"," ");
			printf("%d", (col->size));
			printf("%s", " ");
			printf("%d", (col->price));
			printf("%s","\n");
			//private print here
			}
			}
			else if(pub==4 && isIn==0){
			printf("-----------           _________         \n");
			printf("|         |          |NEED ART!|        \n");      
			printf("|    |OPEN|          O |                \n");
			printf("|   ___   |         -|-|                \n");
			printf("|+++| |+++|          /\\                \n");
			}
			isIn=0;
			warehouselist=(warehouselist->next_warehouse);
			}

	currentWaresf=(currentWaresf->sf_next_warehouse);
	}
	sizelist=sizelist->next;
	}

	return 1;
}



//private = 1 if private, 0 if public.
int addWarehouse(struct linkedlist* head, struct warehouse* newWare,int size,int priv){
	int x =1;
	//Before we do anything, set meta_info.
	uint64_t meta=(uint64_t) size;
	meta=meta<<2;
	//I THINK IT DEFAULTS TO PRIVATE
	uint64_t pri = (uint64_t) priv;
	meta=meta|pri;

	//Base Case: Linkedlist is empty.
	if(head->headlist==NULL){
	
	struct warehouse_list* newHead = (struct warehouse_list*)malloc(sizeof(struct warehouse_list));
	newHead->meta_info=meta; 
	newHead->warehouse=newWare;
	newHead->next_warehouse=NULL;

	struct warehouse_sf_list* sfHead=(struct warehouse_sf_list*) malloc(sizeof(struct warehouse_sf_list));
	sfHead->class_size=size;
	sfHead->warehouse_list_head=newHead;
	sfHead->sf_next_warehouse=NULL;	

	head->headlist=sfHead;
	head->next=NULL;
//	printf("NEW LINKED LIST CREATED \n");
	return 1;
	}

	struct linkedlist* currNode=head;
	while(x==1){

	//If an sf_list with the same size exists, we append it to the list that sf_list points to.
	//If it does not, we simply return 0. We will then create a new sf_list and call addNodeList
	//in the filehandle class.
	if((currNode->headlist->class_size)==size){

	struct warehouse_list* wl=currNode->headlist->warehouse_list_head;
	while(wl->next_warehouse!=NULL){
	wl=wl->next_warehouse;
	}
	struct warehouse_list* newlist= (struct warehouse_list*)malloc(sizeof(struct warehouse_list*));
	//FIGURE OUT META INFO
	newlist->warehouse=newWare;
	newlist->next_warehouse=NULL;
	wl->next_warehouse=newlist;
//	printf("APPENDED TO LIST \n");
	return 1;
	}
		if(currNode->next!=NULL){	
		currNode=currNode->next;
		}
		else{
		x=0;
		}
	}
	//At this point we know we have an actually populated linkedlist,
	//and that node with the same size exists. So now we create a new sf list,
	//warehouselist and linkedlist.
	struct warehouse_list* nl=(struct warehouse_list*)malloc(sizeof(struct warehouse_list));
	nl->meta_info=meta; //yep gotta figure out that one.
	nl->warehouse=newWare;
	nl->next_warehouse=NULL;

	struct warehouse_sf_list* sfnew=(struct warehouse_sf_list*)malloc(sizeof(struct warehouse_sf_list));
	sfnew->class_size=size;
	sfnew->warehouse_list_head=nl;
	sfnew->sf_next_warehouse=NULL;
	
	struct linkedlist* newll=(struct linkedlist*)malloc(sizeof(struct linkedlist));
	newll->headlist=sfnew;
	newll->next=NULL;
	currNode->next=newll; //this should work since currNode is declared within the same scope.
	
//	printf("NEW SF LIST CREATED \n");
	return 1;
}

int addArt(struct linkedlist* head,struct art_collection* art,int size){
//commented out debugging print statements.
	uint64_t zero=(uint64_t) 0;
	struct linkedlist* currNode=head;
	while(currNode!=NULL){
//	printf("%d",currNode->headlist->class_size);
//	printf("%s","\n");
	if((currNode->headlist->class_size)>=size){
//	printf("%s","COMPATIBLE");
	//We have list for size.
	struct warehouse_list* warelist=currNode->headlist->warehouse_list_head;
	while(warelist!=NULL){
	uint64_t occ=warelist->meta_info;
	
//	printf("%s","\n");
//	printf("%lu", (occ>>1)&1);
//	printf("%s","\n");

	if(((occ>>1 )&1)==zero){
	occ=occ|2;
	warelist->meta_info=occ;
	warelist->warehouse->art_collection=art;

	return 0;
	}
	else{
//	printf("%s","Occupied.");
	}
	warelist=warelist->next_warehouse;
	}
	}

	currNode=currNode->next;
	}
	printf("%s","No Fit for Collection.");
	return -1;
}

int utilization(struct linkedlist* head){
	int numwarehouses=0;
	int numoccupied=0;
	int sizeart=0;
	int sizewarehouses=0;
	uint64_t zero=(uint64_t) 0;
	uint64_t one =(uint64_t) 1;
	uint64_t two= (uint64_t) 2;

	struct linkedlist* currNode=head;
	while(currNode!=NULL){
	struct warehouse_sf_list* currentWaresf=(currNode->headlist);
	while(currentWaresf!=NULL){
	int currSize = currentWaresf->class_size;
	struct warehouse_list* currWl=currentWaresf->warehouse_list_head;

	while(currWl!=NULL){
		numwarehouses++;
		sizewarehouses=sizewarehouses+currSize;
		if(((currWl->meta_info)&two)>0){
		numoccupied++;
		sizeart=sizeart+(currWl->warehouse->art_collection->size);
		}
		
		currWl=currWl->next_warehouse;
			}
	currentWaresf=currentWaresf->sf_next_warehouse;
	}
	currNode=currNode->next;
	}
	printf("%s", "Occupied Warehouses vs Total Warehouses: ");
	printf("%d",numoccupied);
	printf("%s"," to ");
	printf("%d",numwarehouses);
	printf("%s", "\nSize of Art Collections vs Total Warehouse Capacity: ");
	printf("%d",sizeart);
	printf("%s", " to ");
	printf("%d",sizewarehouses);

	return 1;
			}

int addNodeList(struct linkedlist* head,struct warehouse_sf_list* newNode){
	struct linkedlist* currNode=head;
	while(currNode->next!=NULL){
	currNode=currNode->next;
	}
	//malloc a linkedlist here
	struct linkedlist* added = (struct linkedlist*)malloc(sizeof(struct linkedlist*));
	currNode->next=added;
	added->headlist=newNode;
	added->next=NULL;
}
#endif
