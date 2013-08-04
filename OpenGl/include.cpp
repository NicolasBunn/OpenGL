#include "include.h"
#include <iostream>

void affMatrix(glm::mat4 mat,const char* name) {
  std::cerr<<"Mat "<<name<<" : "<<std::endl;
  for(int i=0;i<4;i++) {
    for(int j=0;j<4;j++) {
      std::cerr<<mat[j][i]<<"\t";
    }
    std::cerr<<std::endl;
  }
}

void affVector3(glm::vec3 v,const char* name) {
  std::cerr<<name<<"["<<v.x<<"/"<<v.y<<"/"<<v.z<<"]"<<std::endl;
}

void affVector4(glm::vec4 v,const char* name) {
  std::cerr<<name<<"["<<v.x<<"/"<<v.y<<"/"<<v.z<<"/"<<v.w<<"]"<<std::endl;
}