#include "WindowsManager.hpp"


int main(int argc, char **argv)
{

  WindowsManager *manager(WindowsManager::getInstance(argc, argv));

  int softErrorCode(EXIT_FAILURE);
  int wmErrorCode(manager ? manager->getErrorCode() : WINDOWSMANAGER_ERRCODE_FAILED_INIT);

  if (wmErrorCode != WINDOWSMANAGER_ERRCODE_OK)
  {
    goto __vpl__main__ending;
  }

  while ((wmErrorCode = manager->doLoop()) == WINDOWSMANAGER_ERRCODE_OK);


  switch (wmErrorCode)
  {
  case WINDOWSMANAGER_ERRCODE_LEAVING:
    softErrorCode = EXIT_SUCCESS;
    break;

  default:
    break;
  }

__vpl__main__ending:
  return softErrorCode;

}
