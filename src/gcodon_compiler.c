#include <stdio.h>
#include <stdlib.h>
#include "emboss.h"

#include "soapH.h"
#include "GLANGSoapBinding.nsmap"

#include "soapClient.c"
#include "soapC.c"
#include "../gsoap/stdsoap2.c"
#include "../include/gembassy.h"

int main(int argc, char *argv[]){
  embInitPV("gcodon_compiler",argc,argv,"GEMBASSY","1.0.0");

  struct soap soap;
  struct ns1__codon_USCOREcompilerInputParams params;

  AjPSeqall seqall;
  AjPSeq    seq;
  AjPStr    inseq      = NULL;
  AjPStr    id         = NULL;
  AjBool    translate  = 0;
  AjBool    startcodon = 0;
  AjBool    stopcodon  = 0;
  AjPStr    delkey     = NULL;
  AjPStr    data       = NULL;
  AjBool    accid    = 0;
  AjPStr    filename   = NULL;
  char*     jobid;
  
  seqall     = ajAcdGetSeqall("sequence");
  id         = ajAcdGetString("id");
  translate  = ajAcdGetBoolean("translate");
  startcodon = ajAcdGetBoolean("startcodon");
  stopcodon  = ajAcdGetBoolean("stopcodon");
  delkey     = ajAcdGetString("delkey");
  data       = ajAcdGetString("data");
  accid      = ajAcdGetBoolean("accid");

  if(translate){
    params.translate   = 1;
  }else{
    params.translate   = 0;
  }
  if(startcodon){
    params.startcodon  = 1;
  }else{
    params.startcodon  = 0;
  }
  if(stopcodon){
    params.stopcodon   = 1;
  }else{
    params.stopcodon   = 0;
  }
  params.del_USCOREkey = ajCharNewS(delkey);
  params.data          = ajCharNewS(data);
  params.output        = "f";

  while(ajSeqallNext(seqall,&seq)){  
    soap_init(&soap);

    inseq = NULL;
    if(ajSeqGetFeat(seq) && !accid){
      inseq = getGenbank(seq);
    }else{
      ajStrAppendS(&inseq,ajSeqGetAccS(seq));
    }
 
    char* in0;
    in0 = ajCharNewS(inseq);
    if(soap_call_ns1__codon_USCOREcompiler(&soap,NULL,NULL,in0,&params,&jobid)==SOAP_OK){
      ajStrAssignS(&filename,ajSeqGetNameS(seq));
      ajStrAppendC(&filename,".csv");
      fprintf(stderr,"Retrieving file:%s\n",ajCharNewS(filename));
      if(get_file(jobid,ajCharNewS(filename))==0){
        fprintf(stderr,"Retrieval successful\n");
      }else{
        fprintf(stderr,"Retrieval unsuccessful\n");
      }
    }else{
      soap_print_fault(&soap,stderr);
    }
    
    soap_destroy(&soap);
    soap_end(&soap);
    soap_done(&soap);
  }

  ajSeqallDel(seqall);
  ajSeqDel(seq);
  ajStrDel(inseq);
  ajStrDel(filename);
  
  embExit();
  return 0;
}