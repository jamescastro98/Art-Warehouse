#include <stdio.h>
#include "warehouse.h"
#include <string.h>
#include "filehandle.h"

//PROTOTYPES
/*
struct linkedlist{
	struct warehouse_sf_list* headlist;
       	struct linkedlist next;
};
int addWarehouse(struct linkedlist* head,struct warehouse* newWare,int size,int priv);
int deleteArt(char* artName,struct linkedlist* sizelist);
int printOrdered(struct linkedlist* sizelist, int sizeorprice);
int splitWare(struct linkedlist* sizelist, struct warehouse* ware, int artsize);
void clearMem(struct linkedlist* sizelist);
int printLinked(struct linkedlist* sizelist,int pub);
int addArt(struct linkedlist* head,struct art_collection* art,int size);
int utilization(struct linkedlist* head);
int addNodeList(struct linkedlist* head,struct warehouse_sf_list* newNode);
*/

int main(int argc, char** argv) {
	
	int anyware=0;
	//Declaration for flag strings
	char q[]="-q";
	char s[]="-s";
	char p[]="-p";
	char w[]="-w";
	char a[]="-a";

	struct linkedlist* headlink=(struct linkedlist*)malloc(sizeof(struct linkedlist));
	headlink->headlist=NULL;
	headlink->next=NULL;

	//Before we begin, we should see if the user wants quiet mode	
	if(argc>1){
		int sorp=0; //size =0, price = 1;
		if(strcmp(argv[1],q)==0){
			if(strcmp(argv[2],w)==0){
				int loaded=loadWarehouse(argv[3],headlink);
				if(strcmp(argv[4],a)==0){
					int artloaded=loadArt(argv[5],headlink);			
					if(argc==6){
						printLinked(headlink,0);
						return 0;
					}

					if(strcmp(argv[6],s)==0){
						//need to make sortedprint
						printOrdered(headlink,sorp);
						return 0;
					}

					if(strcmp(argv[6],p)==0){
						sorp=1;
						printOrdered(headlink,sorp);
						return 0;
					}
					
					printf("Improper Formatting.\n");
					return 0;
					}		
			printf("Improper Formatting.\n");
			return 0;
		}
		printf("Improper Formatting.\n");	
		return 0;
		}
	printf("Improper Formatting.\n");
	return 0;
	}
//	struct linkedlist* headlink=(struct linkedlist*)malloc(sizeof(struct linkedlist));
//	headlink->headlist=NULL;
//	headlink->next=NULL;

	int z=1; //always true

	//Declarations for input strings
	char userInput[255];
	char exit[]="exit";
	char help[]="help";
	char prnt[]="printall";
	char prntpub[]="print public";
	char prntpriv[]="print private";
	char asc[]="print ascii";
	char util[]="utilization";


	//These ones will require some type of validation
	char add[]= "add art";
	char del[]="delete art";
	char lw[]="load warehouse";
	char la[]= "load art";

	while(z==1){
		printf("Please input a command> ");
		fgets(userInput,255,stdin);
		userInput[strlen(userInput)-1]='\0';	
		if(strcmp(exit,userInput)==0){
			clearMem(headlink);
			return 0;
		}
		else if(strcmp(help,userInput)==0){
			printf("List of Commands");
			printf("\n print ascii - prints current database contents via fancy ascii characters (EC)");
			printf("\n load warehouse FILENAME - loads warehouse from specified file.\n Please ensure each line in the file is formatted as ID SIZE PUBLIC or PRIVATE");
			printf("\n load art FILENAME - loads art pieces into the database from specified file.\n Please ensure each line in the file is formatted NAME SIZE PRICE.");
			printf("\n printall - prints all data in the database.");
			printf("\n utilization - print statistics about warehouse usage and art collections in the database.");
			printf("\n print public - prints all public data in the database.");
			printf("\n print private - prints all private data in the database.");
			printf("\n exit - quits the program");
			printf("\n add art NAME SIZE PRICE - adds art to collection with those fields.");
		}

		else if(strcmp(prnt, userInput)==0){
		printLinked(headlink,0);
		}

		else if(strcmp(asc,userInput)==0){
		printLinked(headlink,4);
		}
		else if(strcmp(prntpub,userInput)==0){
		printLinked(headlink,1);
		}

		else if(strcmp(prntpriv,userInput)==0){
		printLinked(headlink,2);
		}

		else if(strcmp(util,userInput)==0){
		int myutil=utilization(headlink);
		}

		else if(strstr(userInput,la)!=NULL){
			if(anyware==1){
				int len=(strlen(userInput)-strlen(la));
				char strbuff[len+1];
				memcpy(strbuff,&userInput[(strlen(la)+1)],len);
				strbuff[len]='\0';
				int loadAr=loadArt(strbuff,headlink);
			}
			else{
			printf("ERROR: NO WAREHOUSES LOADED IN YET.\n");
			}
		}

		else if(strstr(userInput,lw)!=NULL){
			int len =(strlen(userInput)-strlen(lw));
			char strbuff[len+1];
			memcpy(strbuff,&userInput[(strlen(lw)+1)],len);
			strbuff[len]='\0';
			int loaded = loadWarehouse(strbuff,headlink);
			
			if(loaded<0){
				printf("Error: File Does not Exist");
			}
			anyware=1;
		}

		else if (strstr(userInput,del)!=NULL){
			int len=strlen((userInput)-strlen(del));
			char strbuff[len+1];
			memcpy(strbuff, &userInput[(strlen(del)+1)],len);
			strbuff[len]='\0';
			int deleted=deleteArt(strbuff,headlink);
			if(deleted==1){
				printf("%s",strbuff);
				printf("%s" ," has been deleted.");
			}
			if(deleted!=1){
				printf("%s", strbuff);
				printf("%s", " does not exist in the datbase and CANNOT be deleted.");
			}
		}

		else if(strstr(userInput,add)!=NULL){
			int len=strlen(userInput)-strlen(add);
			char strbuff[len+1];
			memcpy(strbuff,&userInput[(strlen(add)+1)],len);
			strbuff[len]='\0';
			//Now strbuff is just like a line in our file.
			int index=0;
			while(!(isdigit(strbuff[index]))){
				index++;
			}
			index--;
			strbuff[index]='\0'; //replace space with null terminator.
			char* artname=(char*)malloc(sizeof(char)*index);
			memcpy(artname,strbuff,index);
			index++; //go to digit
			int artsize=atoi(strbuff+index);
			while(strbuff[index]!=' '){
				index++;
			}
			index++;
			int artprice=atoi(strbuff+index);
			struct art_collection* newcol= (struct art_collection*)malloc(sizeof(struct art_collection));
			newcol->name=artname;
			newcol->size=artsize;
			newcol->price=artprice;
			int artsuccess=addArt(headlink, newcol,artsize);
			if(artsuccess==0){
				printf("%s", artname);
				printf("%s", " has been added to the database.");
			}
		}

		else{
			printf("Invalid command. For list of commands, please type help and press enter.");
		}
		printf("%s","\n");
	}
	return 0;
}
