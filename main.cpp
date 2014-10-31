#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern "C"
{
   int (*torque_unixmain)(int argc, const char **argv) = NULL;
   void(*setExePathName)(const char *exePathName) = NULL;
}

int main(int argc, const char **argv)
{
   // assume bin name is in argv[0]
   int len = strlen(argv[0]);
printf("%i %s\n",argc,argv[1]);
   char *libName = new char[len+4]; // len + .so + NUL

   strcpy(libName, argv[0]);
   strcat(libName, ".so");

   // try to load the game lib
   void *gameLib = dlopen(libName, RTLD_LAZY | RTLD_LOCAL);
   delete [] libName;
printf("%s\n",libName);
   if(gameLib == NULL)
   {
      printf("%s\n", dlerror());
      return -1;
   }

   // set the filename of the exe image
   setExePathName = (void(*)(const char *)) dlsym(gameLib, "setExePathName");
   if(setExePathName == NULL)
   {
      printf("%s\n", dlerror());
      return -1;
   }
   setExePathName(argv[0]);

   // try to load the lib entry point
   torque_unixmain = (int(*)(int argc, const char **argv)) dlsym(gameLib, "torque_unixmain");

   if(torque_unixmain == NULL)
   {
      printf("%s\n", dlerror());
      return -1;
   }

   // Go!
   return torque_unixmain(argc, argv);
}
