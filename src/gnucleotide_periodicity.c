#include <stdio.h>
#include <stdlib.h>
#include "emboss.h"

#include "soapH.h"
#include "GLANGSoapBinding.nsmap"

#include "soapClient.c"
#include "soapC.c"
#include "../gsoap/stdsoap2.c"
#include "../include/gembassy.h"
#include "../include/display_png.h"

int main(int argc, char *argv[]){
  embInitPV("gnucleotide_periodicity",argc,argv,"GEMBASSY","1.0.0");

  struct soap soap;
  struct ns1__nucleotide_USCOREperiodicityInputParams params;

  AjPSeqall seqall;
  AjPSeq    seq;
  AjPStr    inseq      = NULL;
  ajint     window     = 0;
  AjPStr    nucleotide = 0;
  AjPStr    output     = NULL;
  AjPStr    accid      = NULL;
  AjPStr    filename   = NULL;
  char*     jobid;

  seqall     = ajAcdGetSeqall("sequence");
  window     = ajAcdGetInt("window");
  nucleotide = ajAcdGetString("nucleotide");
  output     = ajAcdGetString("output");
  accid      = ajAcdGetString("accid");

  params.window     = window;
  params.nucleotide = ajCharNewS(nucleotide);
  params.output     = ajCharNewS(output);
  
  while(ajSeqallNext(seqall,&seq)){

    soap_init(&soap);

    inseq = NULL;

    if(ajSeqGetFeat(seq) && !strlen(ajCharNewS(accid))){
      inseq = getGenbank(seq,ajSeqGetFeat(seq));
    }else{
      if(!strlen(ajCharNewS(accid))){
        fprintf(stderr,"Sequence does not have features\n");
        fprintf(stderr,"Proceeding with sequence accession ID\n");
        ajStrAssignS(&inseq,ajSeqGetAccS(seq));
      }
      if(!valID(ajCharNewS(accid))){
          fprintf(stderr,"Invalid accession ID, exiting");
          return 1;
      }else{
        ajStrAssignS(&inseq,accid);
      }
    }

    char* in0;
    in0 = ajCharNewS(inseq);

    fprintf(stderr,"%s\n",ajCharNewS(ajSeqGetAccS(seq)));


    if(soap_call_ns1__nucleotide_USCOREperiodicity(&soap,NULL,NULL,in0,&params,&jobid)==SOAP_OK){
      ajStrAssignS(&filename,ajSeqGetNameS(seq));
      if(strcmp(params.output,"f") == 0){
	ajStrAppendC(&filename,".csv");
      }else{
	ajStrAppendC(&filename,".png");
      }
      if(get_file(jobid,ajCharNewS(filename))==0){
        fprintf(stderr,"Retrieval successful\n");

        if(strcmp(ajCharNewS(output),"show") == 0)
          if(display_png(ajCharNewS(filename), argv[0], ajCharNewS(ajSeqGetAccS(seq))))
            fprintf(stderr,"Error in X11 displaying\n");
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

  ajSeqallDel(&seqall);
  ajSeqDel(&seq);
  ajStrDel(&inseq);
  ajStrDel(&filename);
  
  embExit();
  return 0;
}