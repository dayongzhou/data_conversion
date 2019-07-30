//
//  main.cpp
//  This is to convvert data download from https://www.alphavantage.co/documentation/
//  into
//
//  Created by Zhou, Dayong on 7/19/19.
//  Copyright © 2019 Zhou, Dayong. All rights reserved.
//

#include <iostream>
#include <string.h>

void convert_data(int month, int day, int year);

int main(int argc, const char * argv[]) {
    // insert code here...
    
    //////////////////////////////////////
    // read data for yahoo download and convert it to my needed data format
    // 1. only extract my needed data open low high close
    // 2. reverse the order.
    ///////////////////////////////////////
    
    std::cout << "This file is to operate DDM file\n";
    char filename[25];
    char filename_in[25];
    char filename_out[25];
    char file_csv[5]=".csv";
    char file_txt[5]=".txt";
    char date_lo[35];
    char date_old[35]="123456789";
    char time_lo[12];
    float time_lof;
    FILE *fp;
    FILE *fw;
    float op,lo,hi,cl,ad;
    int vol;
    char line[60];
    int month;
    int year;
    int day;
    int pos[2000] = {0};
    int count = 0;
    int hour;
    int minute;
    int second;
    int day_old = 0;
    int data_count = 0;
    
    
    printf("Input filename:\n");
    scanf("%s",filename);
    sprintf(filename_in, "%s",filename);

    //*filename="HistoricalData.txt";
    
    fp=fopen(filename_in,"r");
    
    if(fp==NULL)
    {
        fprintf(stdout,"%s does not exist",filename_in);
        return(1);
    }

    
    
    
    fscanf(fp,"%[^\n]",line);  // get a line
    
    
    while(!feof(fp))
    {
        
        
        //ftell(fp);
        //fseek(fp, 0, SEEK_END);
        //ftell(fp);
        
        pos[count] = ftell(fp);
        fscanf(fp, "%s %d:%d:%d,%f,%f,%f,%f,%f,%d\n", &date_lo,&hour,&minute,&second,&op,&hi,&lo,&cl,&vol);
        //fprintf(stdout,"%s %d:%d:%d\t%f\t%f\t%f\t%f\t%d\n",date_lo,hour,minute,second,op,lo,hi,cl,vol);
        //fprintf(stdout,"%d\t%d\n",ftell(fp),count);
        sscanf(date_lo,"%d-%d-%d",&year,&month,&day);

            data_count++;
    
            
         if(day_old != day)
         {
       
             if(day_old!=0)
             {
                 fprintf(stdout,"%s data has a length of: %d\n", date_old,data_count);
                 data_count = 1;
             }
             day_old = day;
             sprintf(date_old,"%s",date_lo);
             
        
         }
        
        
        count++;
        
        //ftell(fp);
        
        // fprintf(fw,"%d/%d/%d\t%f\t%f\t%f\t%f\t%d\n",month,day,year,op,lo,hi,cl,vol);
        
        
        
        
        
    }
    fprintf(stdout,"%s data has a length of: %d\n", date_lo,data_count);
    day_old = 0;
    for(int i = count-1; i>=0; i--)
    {
        fseek(fp,pos[i],0);
        fscanf(fp, "%s %d:%d:%d,%f,%f,%f,%f,%f,%d\n", &date_lo,&hour,&minute,&second,&op,&hi,&lo,&cl,&vol);
        sscanf(date_lo,"%d-%d-%d",&year,&month,&day);
        if(day_old!=day)
        {
            sprintf(filename_out,"ddm-%s.txt",date_lo);
            if(day_old!=0)
                fclose(fw);
            day_old = day;

            if((fw = fopen(filename_out,"r"))!=NULL) //check if the file exists or not
            {
                // file exists,
                fclose(fw);
                 sprintf(filename_out,"ddm-%s-2.txt",date_lo);
                
            }
            

            fw = fopen(filename_out,"w");
        }
        fprintf(fw,"%f\n", op);
        fprintf(fw,"%f\n", hi);
        fprintf(fw,"%f\n", lo);
        fprintf(fw,"%f\n", cl);
    
    
        
        
        

    }
    
    fclose(fp);

    

    
    return 0;
}

//////////////////////////////////////
// according to the data to find the directory and read ddm.csv
// then convert to the data format that I needed
// 1. find the directory
// 2. open and scan the date
// 3. extract the date only when market is open
// 4. get open, low, high and close data into a single file
// 6. put data information into the file
///////////////////////////////////////

void convert_data(int month, int day, int year)
{
    
    char filename[35];
    char filename_out[35];
    FILE *fp, *fw;
    
    
    int time_t = 0;
    int open = 0;
    int high = 0;
    int low  = 0;
    int close = 0;
    int volume = 0;
    int adjust = 0;
    int outbound_count = 0;
    int starttime = (9*60*60+60*60/2)*1000;
    int endtime = (16*60*60)*1000;
    
    if(day>=10&&month>=10)
        
        sprintf(filename, "./ddm_csv/%4d%d%d/ddm.csv",year,month,day);
    else if(day<10&&month>=10)
        sprintf(filename, "./ddm_csv/%4d%d0%d/ddm.csv",year,month,day);
    else if(day>=10&&month<10)
        sprintf(filename, "./ddm_csv/%4d0%d%d/ddm.csv",year,month,day);
    else
        sprintf(filename, "./ddm_csv/%4d0%d0%d/ddm.csv",year,month,day);
    
    sprintf(filename_out,"./output/data-%d-%d-%d.txt",month,day,year);
    
    
    fp  = fopen(filename,"r");
    if(fp!=NULL)
    {
        fw = fopen(filename_out,"w");
        while(!feof(fp))
        {
            fscanf(fp, "%d,%d,%d,%d,%d,%d,%d\n", &time_t,&open,&high,&low,&close,&volume,&adjust);
            if(time_t>=starttime&&time_t<=endtime)
                //fprintf(stdout,"%d,%d,%d,%d,%d,%d,%d\n", time_t,open,high,low,close,volume,adjust);
            {
                fprintf(fw,"%f\n", open/10000.0);
                fprintf(fw,"%f\n", high/10000.0);
                fprintf(fw,"%f\n", low/10000.0);
                fprintf(fw,"%f\n", close/10000.0);
                
            }
            else
                outbound_count++;
            
        }
        if(outbound_count>200)
            
            fprintf(stdout,"out of bound = %d for %d/%d/%d\n", outbound_count,year,month,day);
        fclose(fw);
    }
    else
    {
        fclose(fp);
        fprintf(stdout,"no data availabl");
    }
    
    
    
}

