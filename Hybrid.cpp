#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#define PATH "wireless.txt"
#define MAX 9999
using namespace std;

//Global data stuctures 
struct node{
	struct node *link;
	int data;
};
int **arrr,n,*RS,***path,**hopcount,*dist,*stack,top=0,**Traffic,*RSA,***wave,W,*Transmitter,*Receiver,TR,**BS,URG;

// Modules for simulation 

int checkWave(int ,int );//check wavelength for new type of request 
int *reverse(int *);// To reverse any 1d array 
int **sortReq(int **,int );//To sort the request according to their bandwidth 
void RSattachment();//Every Relay station is going to be attached with any of the base station
int **creat();//To Take input of the network topology status 
void display(int **,int );//can display any 2d matrix
void storepath();//To store all the paths 
void SPT(int ,int );// To find shortest path between any two node in any network 
int **creat_traffic();//To generate any random traffic in different type of network
int rand_int(int , char );//randomization function
void modify_traffic();//Modify the input traffic stats deviding them into different Base Station
void creat_wave(int );//It assigns wavelength according to the network topology and resources 
void disp1(int );//Display any 3d matrix
int chooseReq(int *,int *);//it can choose request depending on different strategy 
int findBaseStation(int ,int);//it helps any wireless station to find its base station 
int **copy(int **);//it copies any any 2d matrix
double FindDifference(int **,int **);//Find differences between input traffic matrix and satisfied matrix 
void Routing();//Routing algorithm for any sepecific strategy 
void modifyRunningFrame(int ,int ,int ,int );//modify  frame for any reserved reources in case of subtrails
void modifyRunningWave(int ,int ,int ,int );////modify  frame for any reserved reources in case of subtrails
int checkRunningWave(int ,int ,int,int );//check for avaibility in wavelength any reserved network resources
int checkTrRe(int ,int );//check avaibility for transmitter and receiver in reerved resources
int checkFrame(int ,int ,int );//check availibity for frame in reserved resources 
int changeOCtoFrame(int ,int ,int );//change oc request of any optical network to Frame of WiMax Network
void AssignFrame();//Assign frame value in wimax network 
void AssignTR(int );//Assign  Transmitter Receiver for new request
int findBaseStationStatus(int ,int );//Find base station for any node in network



int main()
{
  // Taking input for  network topology 
    arrr=creat();
    storepath();
    RSattachment();
  // Taking input for traffic
    Traffic = creat_traffic();
    modify_traffic();
    int **Traffic1=copy(Traffic);
 
  //Routing	
	W=7; TR=7;
	Routing();
    cout<<" \n WavelengthNo == "<<W<<"  Difference == "<<FindDifference(Traffic,Traffic1)<<"\n";

}



//Defenetion of modules 

int findBaseStationStatus(int source,int dest)
{
	
	int *pathStoD=path[source][dest];
	int i=0,flag=-1;
	while(pathStoD[i]!=MAX)
	{
		if(RS[pathStoD[i]] == 2)
		flag=pathStoD[i];
		i++;
	}
	if(RS[source]==2 && source!=flag)
	{
		URG=1;
		RS[source]=0;	
	}
	return flag;
}


double FindDifference(int **Traffic,int **Traffic1)
{
	double sum=0,sum1=0;
	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	if(Traffic[i][j]<0)
	sum+=(double)Traffic[i][j];
	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	sum1+=(double)Traffic1[i][j];
	cout<<"\n Total traffic ==  "<<sum1<<" Total Satisfied == "<<sum1+sum<<"\n";
	return (sum1+sum)/sum1;
}


int **copy(int **traffic3)//copy a 2d matrix
{

	int i,j,**traffic2;
	traffic2=(int **)malloc(sizeof(int *)*n);
	for(i=0;i<n;i++)
	traffic2[i]=(int *)malloc(sizeof(int)*n);
	for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	traffic2[i][j]=traffic3[i][j];
	return traffic2;
}


void AssignTR(int amount)
{
	Transmitter = (int * )malloc(sizeof(int )*n);
	Receiver = (int *)malloc(sizeof(int)*n);
	for(int i=0;i<n;i++)
	{
		if(RS[i]!=1)
		{
			Transmitter[i]=amount;
		    Receiver[i]=amount;
		}
		else{
		Transmitter[i]=-1;
		Receiver[i]=-1;
		}
	}
}


int checkWave(int source,int dest)
{
   int i,flag,flag1,*tempPath= path[source][dest];
  	if(RS[source]==1 && RS[dest]==0)
    {
	for(int k=0;k<W;k++)
    {
   	    int BaseStation = findBaseStationStatus(source,dest);
		
		i=0;flag=1,flag1=1;
        while(tempPath[i]!=MAX)
	    {
	    if(tempPath[i]==BaseStation)
	    break;
		i++;
	    }
	    i++;
		while(tempPath[i]!=MAX)
	    {
		if(wave[k][tempPath[i]][tempPath[i-1]]==96)
	    flag++;
	    flag1++;
		i++;
	    }
      if(flag==flag1)
	  return k;
    }}
	else if(RS[source]==0 && RS[dest]==1)
    {
	for(int k=0;k<W;k++)
    {
   	    i=1;flag=1;
        while(RS[tempPath[i]]!=1)
	    {
	  	if(tempPath[i]==MAX)
	  	break;
	    if(wave[k][tempPath[i]][tempPath[i-1]]==96)
	    flag++;
		i++;
	    }
      if(flag== i)
	  return k;
    }}
    
    else if((RS[source]==0||RS[source]==2) && (RS[dest]==0||RS[dest]==2))
    {
    	for(int k=0;k<W;k++)
     {
   	    i=1;flag=1;
        while(tempPath[i]!=MAX)
	    {
	    if(wave[k][tempPath[i]][tempPath[i-1]] == 96)
	    flag++;
		i++;
	    }
        if(flag == i)
	    return k;
        
	 }
	}
	else if((RS[source]==1||RS[source]==2) && (RS[dest]==1||RS[dest]==2)) return 1;
	return -1;
}



int **sortReq(int **cb,int cbPointer){
	for(int j=cbPointer-2;j>=0;j--)
	for(int i=1;i<=j;i++)
	{
		if(cb[i][0]<cb[i+1][0])
		{
			int temp=cb[i][0];
			cb[i][0]=cb[i+1][0];
			cb[i+1][0]=temp;
		    temp=cb[i][1];
			cb[i][1]=cb[i+1][1];
			cb[i+1][1]=temp;
		    temp=cb[i][2];
			cb[i][2]=cb[i+1][2];
			cb[i+1][2]=temp;
		}
	}
	return cb;
}


void AssignFrame()
{
   	 BS = (int **)malloc(sizeof(int *)*n);
     for(int i=0;i<n;i++)
	 BS[i]=(int *)malloc(sizeof(int)*4);
	 for(int i=0;i<n;i++)
	 {
	 	if(RS[i]==2)
	 	{
	 	 BS[i][0]=800;
	 	 BS[i][1]=200;
		 BS[i][2]=800;
	 	 BS[i][3]=200;
		}
		else
		{
		BS[i][0]=0;
	 	BS[i][1]=0;
	    }
	 }
}



int changeOCtoFrame(int source,int Bs,int destination)
{
	int *path1 = path[source][destination];
	int HC=0;
	if(RS[source]==1)
	{
	  for(int i=0;path1[i]!=Bs;i++)
	  {
			HC++;
	  }
	}
	if(RS[destination]==1)
	{
	  for(int i=0;path1[i]!=MAX;i++)
	  {
	  	    if(path1[i]==Bs|| HC)
			HC++;
	  }
	 HC--;
	}
	if(HC-1 == 1)
	return Traffic[source][destination];
	if(HC-1>1 && HC-1<=3)
	return Traffic[source][destination]*2;
	else
	return Traffic[source][destination]*4;
}
 
 
int checkFrame(int source,int Bs,int dest)
{
    if((RS[source]==0||RS[source]==2)&&(RS[dest]==0||RS[dest]==2))
    return 1;
    else if( (RS[source]==0||RS[source]==2) && RS[dest]== 1)
	{
	if(BS[Bs][1]>=(changeOCtoFrame(source,Bs,dest)%10) && BS[Bs][0]>=(changeOCtoFrame(source,Bs,dest) - changeOCtoFrame(source,Bs,dest)%10))
	return 1;
    }
	else if( RS[source]==1 && (RS[dest]== 0||RS[dest]== 2))
	{
	if(BS[Bs][3]>=(changeOCtoFrame(source,Bs,dest)%10) && BS[Bs][2]>=(changeOCtoFrame(source,Bs,dest) - changeOCtoFrame(source,Bs,dest)%10))
	return 1;
    }
	return 0;
}



int checkTrRe(int source,int dest)
{
	int BaseStation = findBaseStationStatus(source,dest);
	if( (RS[source]==0|| RS[source]==2) && (RS[dest]==0|| RS[dest]==2) && Transmitter[source]>0 && Receiver[dest]>0)
	return 1;
	if( RS[source]==0 && RS[dest]==1 && Transmitter[source]>0 && Receiver[BaseStation]>0)
	return 1;
	if( RS[source]==1 && RS[dest]==0 && Receiver[dest]>0 && Transmitter[BaseStation]>0)
	return 1;
	else
	return 1;
	return 0;
}



int checkRunningWave(int amount,int source,int dest,int WaveLengthNo)
{
	if(source==dest)
	return 0;
	int *path1=path[source][dest];
    int flag1=0,flag2=0;
	for(int i=1;path1[i]!=MAX;i++)
	{
		if(wave[WaveLengthNo][path1[i]][path1[i-1]]>=amount)
		flag1++;
		flag2++;
	}
	if(flag1==flag2)
	return 1;
	return 0;
}



void modifyRunningWave(int amount,int source,int dest,int WaveLengthNo)
{
	int *path1=path[source][dest];
	for(int i=1;path1[i]!=MAX;i++)
	{
	wave[WaveLengthNo][path1[i]][path1[i-1]] =wave[WaveLengthNo][path1[i]][path1[i-1]] - amount;
    }
}




void modifyRunningFrame(int source,int BaseStation,int dest,int link)
{
	if(link)
	{
	  	BS[BaseStation][1] = BS[BaseStation][1] - changeOCtoFrame(source,BaseStation,dest)%10;
	  	BS[BaseStation][0] = BS[BaseStation][0] - (changeOCtoFrame(source,BaseStation,dest)-changeOCtoFrame(source,BaseStation,dest)%10);
	}
	if(!link)
	{
	  	BS[BaseStation][3] = BS[BaseStation][3] - changeOCtoFrame(source,BaseStation,dest)%10;
	  	BS[BaseStation][2] = BS[BaseStation][2] - (changeOCtoFrame(source,BaseStation,dest)-changeOCtoFrame(source,BaseStation,dest)%10);
	}
}


void Routing()
{
	int source,dest,h=n*n,BaseStation,opticalEnd;
    creat_wave(W);
    AssignTR(TR);
    AssignFrame();
   
	while(h--)
	{
		
			if(!chooseReq(&source,&dest)) return;
			BaseStation = findBaseStationStatus(source,dest);
		    //cout<<" Hello Source ==  "<<source<<" RS[source] = ="<<RS[source]<<" dest == "<<dest<<" RS[dest] = ="<<RS[dest]<<"\n";
		  //	cout<<checkWave(source,dest)<<"  "<<checkFrame(source,BaseStation,dest)<<" "<<checkTrRe(source,dest)<<"\n";
			if( checkWave(source,dest)>=0 && checkFrame(source,BaseStation,dest) && checkTrRe(source,dest) )
			{
				
			    	
				int **cb,*tempPath,pathLength=0,cbPointer=0;
				cb=(int **)malloc(sizeof(int *)*(n*n));
				for(int i=0;i<(n*n);i++)
				cb[i]=(int *)malloc(sizeof(int )*5);
				for(int i=0;i<(n*n);i++)
				for(int j=0;j<5;j++)
			    cb[i][j]=MAX;
			    tempPath = path[source][dest];
			    for(int i=0;tempPath[i]!=MAX;i++)
			    pathLength++;
				for(int j=pathLength-1;j>=1;j--)
				for(int i=0;i<=j;i++)
				if(Traffic[tempPath[i]][tempPath[j]]>0 )//finds all possible paths for grooming
				{
					cb[cbPointer][0]=Traffic[tempPath[i]][tempPath[j]];
					cb[cbPointer][1]=tempPath[i];
					cb[cbPointer][2]=tempPath[j];
					if((RS[tempPath[i]]==0||RS[tempPath[i]]==2) && (RS[tempPath[j]]==0||RS[tempPath[j]]==2))//Pure Optical
					{
						cb[cbPointer][3] = 0;
						cb[cbPointer][4] = 0;
					}
					if( (RS[tempPath[i]]==0 && RS[tempPath[j]]==1)||(RS[tempPath[i]]==1 && RS[tempPath[j]]==0))//Hybrid
					{
						if( RS[tempPath[i]]==0 && RS[tempPath[j]]==1 )
						{
						 cb[cbPointer][3] = 0;
						 cb[cbPointer][4] = -1;
						}
						if( RS[tempPath[i]]==1 && RS[tempPath[j]]==0 )
						{
						 cb[cbPointer][3] = -1;
						 cb[cbPointer][4] = 0;
						}
					}
					if( (RS[tempPath[i]]==1 && RS[tempPath[j]]==2)||(RS[tempPath[i]]==2 && RS[tempPath[j]]==1))//Pure WiMAX
					{
						cb[cbPointer][3] = -1;
						cb[cbPointer][4] = -1;
					}
					cbPointer++;
				}
				cb = sortReq(cb,cbPointer);
				for(int i=0;i<cbPointer;i++)
				{
					for(int j=0;j<5;j++)
					{
				//		cout<<cb[i][j]<<",";
					}
				//	cout<<"\n";
				}
		     // code from here baby --------------------
		        int WaveLengthNo = checkWave(source,dest);
		     //   cout<<"\n  Wavelength no  == "<<WaveLengthNo<<"\n";
			  for(int i=0;i<cbPointer;i++)
			  {
			        if((RS[cb[i][1]]==0||RS[cb[i][1]]==2) && (RS[cb[i][2]]==0||RS[cb[i][2]]==2))//Pure Optical
					{
			//			cout<<" Pure optical == "<<checkRunningWave(cb[i][0],cb[i][1],cb[i][2],WaveLengthNo)<<"\n";
						if(checkRunningWave(cb[i][0],cb[i][1],cb[i][2],WaveLengthNo) && Transmitter[cb[i][1]]>0 && Receiver[cb[i][2]]>0)
						{
							
							modifyRunningWave(cb[i][0],cb[i][1],cb[i][2],WaveLengthNo);
							cb[i][3] = 1;
						    cb[i][4] = 1;
						    cb[i][0] = 0;
						}
					}
					if( RS[cb[i][1]]==0 && RS[cb[i][2]]==1 )//Hybrid Uplink
						{
			//				 cout<<" Hybrid Uplink "<<checkRunningWave(cb[i][0],cb[i][1],BaseStation,WaveLengthNo)<<" "<<Transmitter[cb[i][1]]<<" "<<Receiver[BaseStation]<<" "<<checkFrame(cb[i][1],BaseStation,cb[i][2])<<"\n";
					     if(checkRunningWave(cb[i][0],cb[i][1],BaseStation,WaveLengthNo) && Transmitter[cb[i][1]]>0 && Receiver[BaseStation]>0 && checkFrame(cb[i][1],BaseStation,cb[i][2]))
						 {
							modifyRunningWave(cb[i][0],cb[i][1],BaseStation,WaveLengthNo);
							modifyRunningFrame(cb[i][1],BaseStation,cb[i][2],1);
							cb[i][3] = 1;
						    cb[i][4] = -1;
						    cb[i][0] = 0;
						 }
						}
					if( RS[cb[i][1]]==1 && RS[cb[i][2]]==0 )//Hybrid Downlink
						{
							if(checkRunningWave(cb[i][0],BaseStation,cb[i][2],WaveLengthNo) && Transmitter[BaseStation]>0 && Receiver[cb[i][2]]>0 && checkFrame(cb[i][1],BaseStation,cb[i][2]))
						  {

						    modifyRunningWave(cb[i][0],BaseStation,cb[i][2],WaveLengthNo);
							modifyRunningFrame(cb[i][1],BaseStation,cb[i][2],0);
							cb[i][3] = -1;
						    cb[i][4] = 1;
						    cb[i][0] = 0;
						  }
				       }
					if(RS[cb[i][1]]==1 && RS[cb[i][2]]==2)//WiMAX Downlink
					{
						if(checkFrame(cb[i][1],BaseStation,cb[i][2]))
						{
							modifyRunningFrame(cb[i][1],BaseStation,cb[i][2],0);
							cb[i][0] = 0;
						}
					}
					if(RS[cb[i][1]]==2 && RS[cb[i][2]]==1)//WiMAX Uplink
					{
						if(checkFrame(cb[i][1],BaseStation,cb[i][2]))
					    {
							modifyRunningFrame(cb[i][1],BaseStation,cb[i][2],1);
							cb[i][0] = 0;
						}
					}
			   }
			   
			   for(int i=0;i<cbPointer;i++)
			   {
			   	if(cb[i][0] == 0)
			   	{
			  // 	cout<<"source = == "<<cb[i][1]<<"Dest == "<<cb[i][2]<<"\n";
				Traffic[cb[i][1]][cb[i][2]]	= 0;
				}
			   }
			   for(int i=0;i<n;i++)
				{
					int trans=0 , rec=0;
				  for(int j=0;j<=cbPointer;j++)
				  {
				  	if((cb[j][1]==i)&&(cb[j][0]==0)&& RS[i]==0)
				  	trans=1;
				  	if((cb[j][2]==i)&&(cb[j][0]==0) && RS[i]==0)
					rec=1;
				  }
				  if(trans)
				  Transmitter[i]--;
				  if(rec)
				  Receiver[i]--;
				}
			}
			else
			Traffic[source][dest]=0-Traffic[source][dest];
			if(URG)
			{
				RS[source]=2;
				URG=0;
			}
   }
}




int findBaseStation(int source,int destination)
{
	int *pathStoD=path[source][destination];
	int i=0;
	while(pathStoD[i]!=destination)
	{
		if( RS[pathStoD[i]] == 2 )
		return pathStoD[i];
		i++;
	}
}



void creat_wave(int w)//craets a 3d matrix to store the wavelengths
{
	int i,j,k;
	wave=(int ***)malloc(sizeof(int **)*w);
	for(i=0;i<w;i++)
	{
		wave[i]=(int **)malloc(sizeof(int *)*n);
		for(j=0;j<n;j++)
		wave[i][j]=(int *)malloc(sizeof(int)*n);
	}
	for(k=0;k<w;k++)
	for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	{
	  if(arrr[i][j]!=0 && (RS[i]==0||RS[i]==2) && (RS[j]==0||RS[j]==2))
	  wave[k][i][j]=96;
	  else 
	  wave[k][i][j]=0;
    }
}





void disp1(int w)//display a 3d array
{
	int i,j,k;
	for(i=0;i<w;i++)
	{
		printf("\n");
		for(j=0;j<n;j++)
		{
			printf("\n");
			for(k=0;k<n;k++)
			{
				printf("%4d",wave[i][j][k]);
			}
		}
	}
}




int chooseReq(int *s,int *d)
{
	int max= 0,i1=0,j1=0;
	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	if(hopcount[i][j]>max  && Traffic[i][j]>0 )
	max = hopcount[i][j];
    int max1=0;
    for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	if(hopcount[i][j]==max  && Traffic[i][j]>max1 && Traffic[i][j]>0 )
	{	
	 max1= Traffic[i][j];
	 i1=i;
	 j1=j;
    }
	if(i1==0 && j1==0)
    return 0;
    else
    {
    	*s =i1;
    	*d =j1;
    	return 1;
	}
}




void modify_traffic()
{
	for(int i=0;i<n;i++)
	for(int j=0;j<n;j++)
	{
		if(i!=j && RS[i]==1 && RS[j]==1)
		{
			Traffic[i][RSA[i]]=Traffic[i][RSA[i]] + Traffic[i][j];
			Traffic[RSA[i]][j]=Traffic[RSA[i]][j] + Traffic[i][j];
			Traffic[i][j]=0;
		}
	}
}




void RSattachment()
{
	RSA = new int[n];
	for(int k=0;k<n;k++)
	{
		if(RS[k]==1)
		{
			int min =MAX;
			int res;
			for(int k1=0;k1<n;k1++){
			   if(RS[k1] == 2 && hopcount[k][k1] < min){
			   	 min = hopcount[k][k1];
				 res = k1;
			   }
			}
			RSA[k]=res;
		}
		else
		RSA[k]=-1;
	}
}




int **creat_traffic()//creats a traffic matrix by puting random values
{
	time_t t;
	int **arr,i,j,k;
	srand((unsigned) time(&t));
	arr=(int **)malloc(sizeof(int *)*n);
	for(i=0;i<n;i++)
	arr[i]=(int *)malloc(sizeof(int)*n);
	for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	{
		if( RS[i]==1 || RS[j] == 1 )
		{
	    k=rand_int(12,1);
		if(i==j)
		arr[i][j]=0;
		else
		{
		 if(k<7)
		 arr[i][j]=1;
		 else if(k<9)
		 arr[i][j]=2;
		 else
		 arr[i][j]=3;
	    }
		}
		else{
		k=rand_int(12,1);
		if(i==j)
		arr[i][j]=0;
		else
		{
		 if(k<9)
		 arr[i][j]=36;
		 else if(k<=11)
		 arr[i][j]=48;
		 else
		 arr[i][j]=96;
		}
	   }
	}
	return arr;
}




int rand_int(int a, char refresh)
{ int ret, t, p, s;
 static int size1, *arr;
 if(refresh)
 {
     arr=(int *)malloc(sizeof(int)*a);
     for(s=0; s<a; s++)
         arr[s]=s+1;
     size1=a;
 }
 p=rand()%size1;
 ret=arr[p];
 t=arr[size1-1];
 arr[p]=t;
 arr[size1-1]=ret;
 size1--;
 return ret;
}




int *reverse(int *asd)
{
	int i=0,j,*stack1;
	while(asd[i]!=MAX)
		i++;


		stack1=(int *)malloc(sizeof(int)*n);
		for(j=0;j<n;j++)
		stack1[j]=MAX;
        for(j=0;j<=i-1;j++)
		stack1[j]=asd[i-j-1];
		top=i-1;
		return stack1;

}




void display(int **arr,int n)//display a 2d array
{
	int i,j;
	for(i=0;i<n;i++)
	{
	  printf("\n");
	  for(j=0;j<n;j++)
       printf(" %d  ",arr[i][j]);
    }

}



void SPT(int u,int v)//find shortest path
{
	int *l,*la,lsize=-1,lasize=0,ch,f=0,i,j,k,k1,min2,min1;
	struct node **tree,*p,*q;
	for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	if(arrr[i][j]==0)
	arrr[i][j]=MAX;
	dist=(int *)malloc(sizeof(int)*n);
	for(i=0;i<n;i++)
	dist[i]=MAX;//distance array stores distance of every node from source
	dist[u]=0;
	l=(int *)malloc(sizeof(int)*n);
	for(i=0;i<n;i++)
	{
		if(i!=u)
		{
		 lsize++;
		 l[lsize]=i;
		}

	}
	la=(int *)malloc(sizeof(int)*n);
	la[lasize]=u;
	stack=(int *)malloc(sizeof(int)*n);
	tree=(struct node **)malloc(sizeof(struct node *)*n);
	for(i=0;i<n;i++)
   {
	tree[i]=0;
	stack[i]=MAX;
   }
	min2=MAX;
	while(lsize>=0)//finding of minimum distance node
	{
		min2=MAX;
		for(i=0;i<=lasize;i++)
		{
			ch=la[i];
		    for(j=0;j<n;j++)
		   {
		       if(((arrr[ch][j]+dist[ch])<=min2)&&(dist[j]==MAX))
			    {
				 min2=dist[ch]+arrr[ch][j];
                 k=ch;k1=j;
		     	}
		   }
	    }
	    if(min2!=MAX)//creation of shortest path tree
	    {
	    	dist[k1]=min2;
	    	if(tree[k]==0)
	    	{
	    		p=(struct node *)malloc(sizeof(struct node));
	    		tree[k]=p;
	    		p->data=k1;
	    		p->link=0;
	    	}
	    	else
	    	{
	    		p=tree[k];
	    		while(p!=0)
	    		{
	    			if(p->link==0)
	    			{
	    				q=(struct node *)malloc(sizeof(struct node));
	    				p->link=q;
	    				q->link=0;
	    				q->data=k1;
	    				break;
	    			}
	    			p=p->link;
	    		}
	    	}

	    }
	    if(k1==v)
	    break;
	    lasize++;
	    la[lasize]=k1;
	    for(i=0;i<lsize;i++)//update the value of l[] &&la[]
	    {
	    	if(l[i]==k1)
	    	{
	    	   l[i]=l[lsize];break;
			}
		}
		lsize--;

	}
	top=0;
	stack[top]=v;
	i=0;
	while(i<n)//finding of path from tree using stack
	{
		p=tree[i];
		while(p!=0)
		{
			if(p->data==stack[top])
			{
				top++;
				stack[top]=i;
				i=-1;
			}
			p=p->link;
		}
		if(stack[top]==u)
		break;
		i++;
	}
}





void storepath()
{
	int i,j,k;
	path=(int ***)malloc(sizeof(int **)*n);
	for(i=0;i<n;i++)
	path[i]=(int **)malloc(sizeof(int *)*n);
	hopcount=(int **)malloc(sizeof(int *)*n);
	for(i=0;i<n;i++)
	hopcount[i]=(int *)malloc(sizeof(int )*n);
	for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	{
		if(i!=j)
		{
			SPT(i,j);
			hopcount[i][j]=top;
			path[i][j]=reverse(stack);
		}
		else
		hopcount[i][j]=0;
	}
    /*for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	{
		if(i!=j)
		{

		 printf("\n %d th to %d \n",i,j);
		 stack=path[i][j];
		 k=0;
		 while(stack[k]!=MAX)
		 {
			printf("%4d-->",stack[k]);
			k++;
		 }
	    }
	}*/
}



int **creat()//creats adjacency matrix
{
	char arr[100],c;int rs=0,i=0,j,k,n1,i1,l,**arrr1;FILE *ptr;
	ptr=fopen(PATH,"r");
	fgets(arr,80,ptr);
    i=0;n=0;
	while(arr[i]!='\n')// gets the total no of vertex
	{
		k=arr[i]-'0';
		n=(n*10)+k;
		i++;
	}
	arrr1=(int **)malloc(sizeof(int *)*n);
	for(i=0;i<n;i++)
	arrr1[i]=(int *)malloc(sizeof(int)*n);
	n1=0;i=0;j=0;k=0;
	for(i=0;i<n;i++,j=0)
	{
	 fgets(arr,80,ptr);i1=0;
	 l=strlen(arr)-1;
	  for(i1=0;i1<=l;i1++)
	   {
	     if(isdigit(arr[i1]))
	     {
	      k=0;
	      k=arr[i1]-'0';
	       n1=n1*10+k;
	      if((i==n-1)&&(j==n-1))
	      arrr1[i][j]=n1;
	     }
	    else
        {
         arrr1[i][j]=n1;
	     n1=0;j++;
	    }
	   }
	}
	n1=0;
	fgets(arr,80,ptr);
	l= strlen(arr)-1;
	int *RS1 = new int[n];
	for( i1=0;i1<=l;i1++ )
	    {
	     if( isdigit(arr[i1]) )
	     {
	       k=0;
	       k=arr[i1]-'0';
	       n1=n1*10+k;
	     }
	     else
         {
           RS1[rs++]=n1;
	       n1=0;
	     }
	    }
	   RS = new int[n];
	   for(int k=0;k<n;k++)
	   RS[k]=0;
	   for(int k=0;k<rs;k++)
	   RS[RS1[k]]=1;
	   n1=0;rs=0;
	   fgets(arr,80,ptr);
	   l = strlen(arr)-1;
	   for(i1=0;i1<=l;i1++)
	   {
	     if(isdigit(arr[i1]))
	     {
	      k=0;
	      k=arr[i1]-'0';
	       n1=n1*10+k;
	     }
	    else
        {
         RS1[rs++]=n1;
	     n1=0;
	    }
	   }
	   for(int k=0;k<rs;k++)
	    RS[RS1[k]]=2;
	fclose(ptr);
	return arrr1;
}

