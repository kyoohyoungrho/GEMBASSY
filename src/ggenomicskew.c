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
  embInitPV("ggenomicskew",argc,argv,"GEMBASSY","1.0.0");

  struct soap soap;
  struct ns1__genomicskewInputParams params;

  AjPSeqall seqall;
  AjPSeq    seq;
  AjPStr    inseq      = NULL;
  ajint     divide     = 0;
  AjBool    at         = 0;
  AjPStr    output     = NULL;
  AjPStr    filename   = NULL;
  AjPFile   infile    = NULL;
  AjPStr    line      = NULL;
  int       i         = 0;
  int       j         = 0;
  char*     _result;

  seqall = ajAcdGetSeqall("sequence");
  divide = ajAcdGetInt("divide");
  at     = ajAcdGetBoolean("at");
  output = ajAcdGetString("output");

  params.divide       = divide;
  if(at){
    params.at         = 1;
  }else{
    params.at         = 0;
  }
  params.output       = ajCharNewS(output);

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
    if(soap_call_ns1__genomicskew(&soap,NULL,NULL,in0,&params,&jobid)==SOAP_OK){
      ajStrAssignS(&filename,ajSeqGetNameS(seq));
      if(strcmp(params.output,"g") == 0){
	ajStrAppendC(&filename,".png");
      }else{
	ajStrAppendC(&filename,".csv");
      }
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
