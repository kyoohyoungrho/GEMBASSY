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
  embInitPV("gdelta_enc",argc,argv,"GEMBASSY","1.0.0");
  
  struct soap soap;

  AjPSeqall seqall;
  AjPSeq    seq;
  AjPStr    inseq  = NULL;
  AjPStr    filename   = NULL;
  AjPFile   infile    = NULL;
  AjPStr    line      = NULL;
  int       i         = 0;
  int       j         = 0;
  char*     jobid;

  seqall = ajAcdGetSeqall("sequence");
  
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
    if(soap_call_ns1__delta_USCOREenc(&soap,NULL,NULL,in0,&jobid)==SOAP_OK){
      puts(jobid);
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
