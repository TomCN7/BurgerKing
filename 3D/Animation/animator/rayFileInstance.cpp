#include <stdio.h>
#include <stdlib.h>
#include "rayFileInstance.h"


RayFileInstance::RayFileInstance(RayFile* rf){
  rayFile=rf;
}
char* RayFileInstance::name(void){return "ray file instance";}
void  RayFileInstance::write(int indent,FILE* fp){
  int i;
  for(i=0;i<indent;i++){fprintf(fp," ");}
  fprintf(fp,"#ray_file_instance %d\n",rayFile->index);
}
void RayFileInstance::draw(int complexity){
  rayFile->scene->group->draw(complexity);
}
