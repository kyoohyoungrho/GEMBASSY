#include <stdio.h>
#include <stdlib.h>
#include "emboss.h"

#include "soapH.h"
#include "GLANGSoapBinding.nsmap"

#include "soapClient.c"
#include "soapC.c"
#include "../gsoap/stdsoap2.c"
#include "../include/getfile.h"

int main(int argc, char *argv[]){
  embInitPV("gbui",argc,argv,"GEMBASSY","1.0.0");
  
  struct soap soap;
  struct ns1__buiInputParams params;

  AjPSeqall seqall;
  AjPSeq    seq;
  AjPStr    inseq     = NULL;
  AjBool    translate = 0;
  AjPStr    position  = NULL;
  AjPStr    id        = NULL;
  AjPStr    delkey    = NULL;
  AjPStr    filename  = NULL;
  AjPFile   infile    = NULL;
  AjPStr    line      = NULL;
  int       i         = 0;
  int       j         = 0;
  char*     jobid;
  
  seqall     = ajAcdGetSeqall("sequence");
  translate  = ajAcdGetBoolean("translate");
  position   = ajAcdGetString("position");
  id         = ajAcdGetString("id");
  delkey     = ajAcdGetString("delkey");
  
  if(translate){
    params.translate   = 1;
  }else{
    params.translate   = 0;
  }
  params.position      = ajCharNewS(position);
  params.id            = ajCharNewS(id);
  params.del_USCOREkey = ajCharNewS(delkey);
  
  while(ajSeqallNext(seqall,&seq)){
    soap_init(&soap);

    inseq = NULL;
    if(ajSeqGetFeat(seq)){
      i++;
      ajStrAssignS(&filename,ajSeqallGetFilename(seqall));
      if(infile == NULL)
        infile = ajFileNewInNameS(filename);
      while (ajReadline(infile, &line)) {
        ajStrAppendS(&inseq,line);
        if(ajStrMatchC(line,"//\n")){
          j++;
          if(i == j)
            break;
        }
      }
    }else{
      ajStrAppendS(&inseq,ajSeqGetAccS(seq));
    }

    char* in0;
    in0 = ajCharNewS(inseq);
    if(soap_call_ns1__bui(&soap,NULL,NULL,in0,&params,&jobid)==SOAP_OK){
      ajStrAssignS(&filename,ajSeqGetNameS(seq));
      ajStrAppendC(&filename,".csv");
      if(get_file(jobid,ajCharNewS(filename))==0){
	printf("Retrieval successful\n");
      }else{
	printf("Retrieval unsuccessful\n");
      }
    }else{
      soap_print_fault(&soap,stderr);
    }
  
    soap_destroy(&soap);
    soap_end(&soap);
    soap_done(&soap);
  }
  
  embExit();
  return 0;
}
