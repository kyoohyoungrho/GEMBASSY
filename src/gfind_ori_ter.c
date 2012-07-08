#include <stdio.h>
#include <stdlib.h>
#include "emboss.h"

#include "soapH.h"
#include "GLANGSoapBinding.nsmap"

#include "soapClient.c"
#include "soapC.c"
#include "../gsoap/stdsoap2.c"

int main(int argc, char *argv[]){
  embInitPV("gfindoriter",argc,argv,"GEMBASSY","1.0.0");

  struct soap soap;
  struct ns1__find_USCOREori_USCOREterInputParams params;

  AjPSeq    seq;
  AjPStr    inseq      = NULL;
  ajint     window     = 0;
  AjBool    purine     = 0;
  AjBool    keto       = 0;
  ajint     filter     = 0;
  AjPStr    mode  = NULL;
  char*     _result; 
  char*     jobid;

  seq    = ajAcdGetSeq("sequence");
  window = ajAcdGetInt("window");
  filter = ajAcdGetInt("filt");
  purine = ajAcdGetBoolean("purine");
  keto   = ajAcdGetBoolean("keto");
  mode = ajAcdGetString("mode");

  params.window   = window;
  params.filter   = filter;
  if(purine){
    params.purine = 1;
  }else{
    params.purine = 0;
  }
  if(keto){
    params.keto   = 1;
  }else{
    params.keto   = 0;
  }
    
  soap_init(&soap);
  
  inseq = NULL;
  ajStrAppendS(&inseq,ajSeqGetNameS(seq));
  
  char* in0;
  in0 = ajCharNewS(inseq);
  if(soap_call_ns1__find_USCOREori_USCOREter(&soap,NULL,NULL,in0,&params,&jobid)==SOAP_OK){
    char* dlm = "<>";
    char* tp  = jobid;
    printf("origin\tterminus\n");
    tp = strtok(tp,dlm);
    tp = strtok(NULL,dlm);
    printf("%s\t",tp);
    tp = strtok(NULL,dlm);
    tp = strtok(NULL,dlm);
    tp = strtok(NULL,dlm);
    printf("%s\n",tp);
  }else{
    soap_print_fault(&soap,stderr);
  }
  
  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);
  
  embExit();
  return 0;
}
