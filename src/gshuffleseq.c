#include <stdio.h>
#include <stdlib.h>
#include "emboss.h"

#include "soapH.h"
#include "GLANGSoapBinding.nsmap"

#include "soapClient.c"
#include "soapC.c"
#include "../gsoap/stdsoap2.c"

char *_upper(char *s){
  char *p;
  
  for(p=s; *p; p++){
    *p = toupper(*p);
  }
  return(s);
}

int main(int argc, char *argv[]){
  embInitPV("gshuffleseq",argc,argv,"GEMBASSY","0.0.1");
  
  struct soap soap;
  struct ns1__shuffleseqInputParams params;
  
  AjPSeqout seqout;
  AjPSeq    seq;
  AjPStr    inseq      = NULL;
	AjPStr    filename   = NULL;
  ajint     k          = 0;
  AjPStr    mode  = NULL;
  char*     jobid;
  
  seq    = ajAcdGetSeq("sequence");
  k      = ajAcdGetInt("k");
  seqout = ajAcdGetSeqout("outseq");
  
  params.k = k;
  
  soap_init(&soap);
  
  inseq = NULL;
  seq=ajAcdGetSeq("sequence");
  ajStrAppendS(&inseq,ajSeqGetSeqS(seq));
  ajStrAppendS(&filename,ajSeqGetNameS(seq));
  
  char* in0;
  in0 = ajCharNewS(inseq);
  if(soap_call_ns1__shuffleseq(&soap,NULL,NULL,in0,&params,&jobid)==SOAP_OK){
		printf("Writing sequence out to %s\n",ajCharNewS(filename));
		ajCharFmtUpper(jobid);
    ajSeqAssignSeqC(seq, jobid);
    ajSeqoutWriteSeq(seqout, seq);
    ajSeqoutClose(seqout);
  }else{
    soap_print_fault(&soap,stderr);
  }
  
  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);
  
  embExit();
  return 0;
}