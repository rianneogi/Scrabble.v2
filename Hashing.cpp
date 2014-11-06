#include "Hashing.h"

unsigned long MAXHASH = (1<<19); //this value will be the upper limit for the hash
                                 //use hash%MAXHASH as the offset
                                 //bigger this value, the bigger the file, but the lesser the number of collisions

                                 //note: don't use maxhash lower than 165415


//maxhash = (1<<19), Total Collisions:26247, Worst Case:13, File Size: 8.7 MB

unsigned long hash(char* str) //blatantly copied from the internet
{
    unsigned long h = 5381;
    int c;

    while (c = *str++)
    {
        //if(c=='\n') break;
        h = ((h << 5) + h) + c; /* h * 33 + c */
    }

    return h;
}

int createhashfile()
{
    int counter=0,flag=0,i;
    FILE *fp,*fq;
    struct word my_record,buffer;
    fp=fopen("words.txt","r");
    fq=fopen("hash.bin","w+b");
    if(!fp)
    {
        return -1;
    }
    if(!fq)
    {
        return -1;
    }
    for(i=0;i<17;i++)
    {
        my_record.tempw[i] = 0;
    }
    for(i=0;i<MAXHASH;i++)
    {
        fwrite(&my_record,sizeof(struct word),1,fq); //empty entire file
    }
    fseek(fq,0,SEEK_SET);
    while(fgets(my_record.tempw,16,fp)!=NULL)
    {
        if(strstr(my_record.tempw,"\n")!=NULL && strcmp(my_record.tempw,"\n")!=0)
        {
            unsigned long h = hash(my_record.tempw)%MAXHASH;
            fseek(fq,h*sizeof(struct word),SEEK_SET);
            fread(&buffer,sizeof(struct word),1,fq);
            if(strlen(buffer.tempw)!=0) //collision?
            {
                while(1) //continue as long as there are collisions
                {
                    if(fread(&buffer,sizeof(struct word),1,fq)!=1) //reached end of file?
                    {
                        fseek(fq,0,SEEK_SET); //go to beginning
                        continue;
                    }
                    if(strlen(buffer.tempw)==0)
                    {
                        break;
                    }
                }
            }
            fseek(fq,-sizeof(struct word),SEEK_CUR);
            fwrite(&my_record,sizeof(struct word),1,fq); //write
        }
    }
    fclose(fp);
    fclose(fq);
    return 0;
}

//returns 1 if words exists, 0 otherwise
int searchhash(struct word s)
{
    int counter=0,flag=0,i=0,temp;
	unsigned long h;
    struct word my_record,str;
    FILE* ptr_myfile = fopen("hash.bin","rb");

    flag = 0;
    for(i = 0;i<16;i++)
    {
        if(s.tempw[i]=='\0')
        {
            s.tempw[i] = '\n';
            s.tempw[i+1] = '\0';
            break;
        }
    }

    if(!ptr_myfile)
        return -1; //error
    h = hash(s.tempw)%MAXHASH;
    fseek(ptr_myfile,h*sizeof(struct word),SEEK_SET);
    while(1)
    {
        if(fread(&str,sizeof(struct word),1,ptr_myfile)!=1) //end of file?
        {
            fseek(ptr_myfile,0,SEEK_SET); //go to beginning
            continue;
        }
        if(strlen(str.tempw)==0)
        {
            break;
        }
        if(strcmp(str.tempw,s.tempw)==0)
        {
            flag = 1;
            break;
        }
    }
	fclose(ptr_myfile);
    if(flag==1)
       return 1;
    return 0;
}
