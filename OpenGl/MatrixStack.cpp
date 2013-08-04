#include "MatrixStack.hpp"                                                                                                                                                   


MatrixStack::MatrixStack()
{
  _matrixStack.push(glm::mat4(1.f));
}

MatrixStack::~MatrixStack()
{

}


void MatrixStack::push(glm::mat4 value)
{
  _matrixStack.push(value);
}

glm::mat4 MatrixStack::pop()
{
  glm::mat4 backup(_matrixStack.top());

  _matrixStack.pop();
  return backup;
}


glm::mat4 MatrixStack::top()
{
  return _matrixStack.top();
}

void MatrixStack::settop(glm::mat4 value)
{
  _matrixStack.top() = value;
}

void MatrixStack::mult(glm::mat4 value)
{
  _matrixStack.top() *= value;
}

void MatrixStack::translate(glm::vec3 value)
{
  _matrixStack.top() = glm::translate(_matrixStack.top(), value);  
}

void MatrixStack::rotate(float angle, glm::vec3 value)
{
  _matrixStack.top() = glm::rotate(_matrixStack.top(), angle, value);
}

void MatrixStack::scale(glm::vec3 value)
{
  _matrixStack.top() = glm::scale(_matrixStack.top(), value);
}

