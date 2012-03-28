#include <stdio.h>
#include <stdlib.h>
#include "emboss.h"

#include "soapH.h"
#include "GLANGSoapBinding.nsmap"

#include "soapClient.c"
#include "soapC.c"
#include "../gsoap/stdsoap2.c"

int main(int argc, char *argv[]){
  embInitPV("ggcsi",argc,argv,"GEMBASSY","0.0.1");

  struct soap soap;
  struct ns1__gcsiInputParams params;
  
  AjPSeq    seq;
  AjPStr    inseq      = NULL;
  ajint     window     = 0;
  ajint     version    = 0;
  AjBool    at         = 0;
  AjBool    purine     = 0;
  AjBool    keto       = 0;
  AjBool    p;
  AjPStr    outstring  = NULL;
  char*     jobid;
  
  seq        = ajAcdGetSeq("sequence");
  window     = ajAcdGetInt("window");
  version    = ajAcdGetInt("v");
  at         = ajAcdGetBoolean("at");
  purine     = ajAcdGetBoolean("purine");
  keto       = ajAcdGetBoolean("keto");
  p          = ajAcdGetBoolean("p");

  params.window       = window;
  params.version      = version;
  if(at){
    params.at         = 1;
  }else{
    params.at         = 0;
  }
  if(purine){
    params.purine     = 1;
  }else{
    params.purine     = 0;
  }
  if(keto){
    params.keto       = 1;
  }else{
    params.keto       = 0;
  }
  if(p){
    params.p          = 1;
  }else{
    params.p          = 0;
  }
    
  soap_init(&soap);
  
  inseq = NULL;
  ajStrAppendC(&inseq,">");
  ajStrAppendS(&inseq,ajSeqGetNameS(seq));
  ajStrAppendC(&inseq,"\n");
  ajStrAppendS(&inseq,ajSeqGetSeqS(seq));
  
  char* in0;
  in0 = ajCharNewS(inseq);
  if(soap_call_ns1__gcsi(&soap,NULL,NULL,in0,&params,&jobid)==SOAP_OK){
    float gcsi,sa,dist;
    int i;
    char* tp  = jobid;
    char* dlm = "<>";
    tp = strtok(tp,dlm);
    tp = strtok(NULL,dlm);
    gcsi = atof(tp);
    for(i=0;i<3;i++){
      tp = strtok(NULL,dlm);
    }
    sa = atof(tp);
    for(i=0;i<3;i++){
      tp = strtok(NULL,dlm);
    }
    dist = atof(tp);
    printf("GCSI\t\tSA\t\tDist\n%f\t%f\t%f\n",gcsi,sa,dist);
  }else{
    soap_print_fault(&soap,stderr);
  }
  
  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);

  embExit();
  return 0;
}