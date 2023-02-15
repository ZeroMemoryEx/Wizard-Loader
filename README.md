# Wizard-Loader

* Xwizard.exe is a commonly used diagnostic tool for Windows setup and installation, and like other executables, it loads dynamic link libraries (DLLs) 
to perform various tasks. However, The PoC patch the Xwizard.exe binary on order to make LoadLibrary API load malicious DLL instead of the intended one.

  ![image](https://user-images.githubusercontent.com/60795188/218910696-f9800828-833b-4354-aa57-3aeeb9a16caf.png)
