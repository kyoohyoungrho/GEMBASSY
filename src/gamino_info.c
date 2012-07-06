#include <stdio.h>
#include <stdlib.h>
#include "emboss.h"

#include "soapH.h"
#include "GLANGSoapBinding.nsmap"

#include "soapClient.c"
#include "soapC.c"
#include "../gsoap/stdsoap2.c"

int main(int argc, char *argv[]){
  embInitPV("gaminoinfo",argc,argv,"GEMBASSY","1.0.0");
  
  struct soap soap;
  
  AjPSeq    seq   = NULL;
  AjPStr    inseq = NULL;
  int i;
  char*     _result;
  
  soap_init(&soap);
  
  inseq = NULL;
  seq=ajAcdGetSeq("sequence");
  ajStrAppendC(&inseq,">");
  ajStrAppendS(&inseq,ajSeqGetNameS(seq));
  ajStrAppendC(&inseq,"\n");
  ajStrAppendS(&inseq,ajSeqGetSeqS(seq));  
    
  char* in0;
  in0 = ajCharNewS(inseq);
  if(soap_call_ns1__amino_USCOREinfo(&soap,NULL,NULL,in0,&_result)==SOAP_OK){
    puts(_result);
  }else{
      soap_print_fault(&soap,stderr);
  }
  
  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);
  
  embExit();
  return 0;
}